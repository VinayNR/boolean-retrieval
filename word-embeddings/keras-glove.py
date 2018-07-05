import os
from os.path import join
import numpy as np

import keras
from keras.preprocessing.text import Tokenizer
from keras.layers import Embedding, Conv1D, MaxPooling1D, Flatten, Dense, GlobalMaxPooling1D
from keras.models import Model

from sklearn.model_selection import train_test_split

import loss

max_words = 20000
embed_dim = 300
max_seq_len = 1000
epochs = 2
batch_size = 512

def load_files(train_dir):
    samples = [] # list of literally all the text samples
    class_index = {} # dictionary to link each class to it's class_id
    labels = [] # holds all class_ids
    index_val = 0
    classes = os.listdir(train_dir)

    for class_ in classes:
        #print(class_)
        if class_ != ".DS_Store" or "desktop.ini":
            path = join(train_dir, class_)
            class_id = index_val
            class_index[class_] = class_id
            for root, dirs, files in os.walk(path):
                for file in files:
                    if not file.endswith('.DS_Store'):
                        file_path = join(path, file)
                        with open(file_path, 'r') as sample:
                            text = sample.read()
                            # skip the initial unwanted header stuff
                            idx = text.find('\n\n')
                            # if header exists means idx will be greater than 0
                            if idx > 0:
                                text = text[idx:]
                            samples.append(text)
                        labels.append(class_id)
            index_val += 1

    print("The total number of texts found are " + str(len(samples)))

    return samples, labels, class_index


def embedding(samples, class_ids):
    # extract stuff from glove output
    embed_index = {}
    i = 0
    with open(join("glove.840B.300d", "glove.840B.300d.txt"), encoding = "utf8") as embed_file:
        for line in embed_file:
            word_vector = line.split()
            token = word_vector[0]
            try:
                vectors = np.asarray(word_vector[1:], dtype = 'float32')
            except ValueError:
                print(i, line)

            embed_index[token] = vectors
            i += 1

    print('There are ' + str(len(embed_index)) + ' word vectors')

    # restrict it to just 25000 most often occuring words/tokens
    tokenizer = Tokenizer(num_words = max_words)
    tokenizer.fit_on_texts(samples)
    seqs = tokenizer.texts_to_sequences(samples)

    token_index = tokenizer.word_index
    print("There are " + str(len(token_index)) + " tokens")

    # restrict each sequence to a max of 1500 words
    data = keras.preprocessing.sequence.pad_sequences(seqs, maxlen = max_seq_len)

    class_ids_np = np.asarray(class_ids)
    # hot encode the class ids
    labels = keras.utils.to_categorical(class_ids_np)

    #split into train and validation subsets for better optimization
    x_train, x_val, y_train, y_val = train_test_split(data, labels, test_size = 0.2, random_state = 11)

    tokens_len = min(max_words, len(token_index) + 1)
    embed_matrix = np.zeros((tokens_len, embed_dim))

    for token, idx in token_index.items():
        if idx >= max_words:
            continue

        embed_vector = embed_index.get(token)
        if embed_vector is None:
            # the words not found in index will be given as all zeros
            continue

        embed_matrix[idx] = embed_vector

    embed_layer = Embedding(tokens_len, embed_dim, weights = [embed_matrix], input_length = max_seq_len, trainable = False)

    return embed_layer, x_train, x_val, y_train, y_val


def create_model(embed_sequences, class_index):
    _ = Conv1D(128, 5, activation = 'relu')(embed_sequences)
    _ = MaxPooling1D(5)(_)
    _ = Conv1D(256, 5, activation = 'relu')(_)
    _ = MaxPooling1D(7)(_)
    _ = Conv1D(256, 5, activation = 'relu')(_)
    _ = GlobalMaxPooling1D()(_)
    _ = Dense(256, activation = 'relu')(_)
    op = Dense(len(class_index), activation = 'softmax')(_)

    return op


if __name__ == "__main__":
    print("Loading the files...")
    samples, labels, class_index = load_files('../Train/')
    print("Pre-processing...")
    embedding_layer, x_train, x_val, y_train, y_val = embedding(samples, labels)
    print("Training...")

    seq_input = keras.Input(shape = (max_seq_len, ), dtype = 'int32')
    embed_seq = embedding_layer(seq_input)
    final = create_model(embed_seq, class_index)

    model = Model(seq_input, final)
    print(model.summary())

    model.compile(loss = 'categorical_crossentropy', optimizer = 'rmsprop', metrics = ['acc'])
    hist = model.fit(x_train, y_train, batch_size = batch_size, epochs = epochs, validation_data = (x_val, y_val))

    loss.plot_accuracy(hist) # plot the accuracy curves
    loss.plot_loss(hist) # plot the loss curves
