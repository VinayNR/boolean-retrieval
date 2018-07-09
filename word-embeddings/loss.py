import pandas as pds
import numpy as np

import keras
from keras.preprocessing.text import Tokenizer
from keras.utils import to_categorical

from sklearn.metrics import classification_report

# uncomment the below lines if using mac OS or MAC OSX, within a virtual env or if using anaconda
# import matplotlib
# matplotlib.use('PS')
import matplotlib.pyplot as plot

max_words = 20000

def plot_accuracy(fit):
    plot.figure(figsize = [8, 8])
    plot.plot(fit.history['val_acc'],'g',linewidth = 2.0)
    plot.plot(fit.history['acc'],'r', linewidth = 2.0)
    plot.legend(['Training Accuracy', 'Validation Accuracy'], fontsize = 14)
    plot.xlabel('epochs ', fontsize = 14)
    plot.ylabel('accuracy', fontsize = 14)
    plot.title('accuracy vs epochs', fontsize = 14)
    # plot.show()
    plot.savefig('epoch_vs_acc.png', transparent = False, bbox_inches = 'tight')


def plot_loss(fit):
    plot.figure(figsize = [8, 8])
    plot.plot(fit.history['val_loss'],'g', linewidth = 2.0)
    plot.plot(fit.history['loss'],'r', linewidth = 2.0)
    plot.legend(['Training loss', 'Validation Loss'], fontsize = 14)
    plot.xlabel('epochs ', fontsize = 14)
    plot.ylabel('loss', fontsize = 14)
    plot.title('loss vs epochs', fontsize = 14)
    # plot.show()
    plot.savefig('epoch_vs_loss.png', transparent = False, bbox_inches = 'tight')

def prediction(samples):
    tokenizer = Tokenizer(num_words = max_words)
    tokenizer.fit_on_texts(samples)
    sequences = tokenizer.texts_to_sequences(samples)
    print(sequences)
