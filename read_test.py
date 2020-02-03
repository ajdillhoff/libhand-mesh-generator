import os

import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
from PIL import Image


def main():
    annotations = pd.read_pickle("./dataset/train/annotations.pkl")
    num_samples = len(annotations)

    idx = np.random.randint(num_samples)

    for i, v in enumerate(annotations[idx]):
        print("{}: ({}, {}, {})".format(v, annotations[idx][v][0],
                                        annotations[idx][v][1],
                                        annotations[idx][v][2]))

    img_file = "./dataset/train/depth/depth_{}.png".format(idx)
    img_handle = Image.open(img_file)
    img = np.array(img_handle)

    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.imshow(img)
    plt.show()


if __name__ == "__main__":
    main()
