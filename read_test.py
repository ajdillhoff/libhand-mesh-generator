import os

import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
from PIL import Image


def main():
    annotations = pd.read_pickle("./dataset/train/annotations.pkl")
    for i, v in enumerate(annotations[0]):
        print("{}: ({}, {}, {})".format(v, annotations[0][v][0], annotations[0][v][1], annotations[0][v][2]))

    img_file = "./dataset/train/depth/depth_1.png"
    img_handle = Image.open(img_file)
    img = np.array(img_handle)

    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.imshow(img)
    plt.show()


if __name__ == "__main__":
    main()
