import os
import pickle

import matplotlib.pyplot as plt
import numpy as np
from PIL import Image


def get_num_samples(path):
    with open(path, mode='rb') as file:
        try:
            num_samples = pickle.load(file)
        except EOFError:
            num_samples = 0
            print("EOFError: File must have been empty.")

    return num_samples


def load_annotation(path, idx):
    cur_idx = 0
    with open(path, mode='rb') as file:
        try:
            while cur_idx < idx:
                pickle.load(file)
                cur_idx += 1
            anno = pickle.load(file)
        except EOFError:
            anno = []
            print("ERROR: EOFError")

    return anno


def main():
    # The number of samples is written to the beginning of the file.
    path = "./train/annotations.pkl"
    num_samples = get_num_samples(path)

    idx = np.random.randint(num_samples)
    anno = load_annotation(path, idx)

    for i, v in enumerate(anno):
        print("{}: ({}, {}, {})".format(v, anno[v][0],
                                        anno[v][1],
                                        anno[v][2]))

    img_file = "./train/depth/depth_{}.png".format(idx)
    img_handle = Image.open(img_file)
    img = np.array(img_handle)

    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.imshow(img)
    plt.show()


if __name__ == "__main__":
    main()

