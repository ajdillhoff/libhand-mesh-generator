import os
import sys

if os.path.abspath("build/src") not in sys.path:
    sys.path.append(os.path.abspath("build/src"))

import matplotlib.pyplot as plt
import numpy as np
import PoseGenerator


def main():
    scene_path = "/home/alex/dev/projects/libhand-public/nyu_synth/scene_spec.yml"
    pose_config_path = "configs/pose/test.json"
    shape_config_path = "configs/shape/train.json"
    pose_gen = PoseGenerator.PoseGenerator(scene_path, pose_config_path, shape_config_path)
    rows = 4
    columns = 4

    fig = plt.figure(figsize=(12, 9))
    for i in range(0, rows * columns):
        sample = pose_gen.GetSample()
        ax = fig.add_subplot(rows, columns, i+1)
        ax.set_xticklabels([])
        ax.set_yticklabels([])
        plt.axis('off')
        ax.imshow(sample.depth)
    plt.subplots_adjust(wspace=0, hspace=0)
    plt.show()


if __name__ == "__main__":
    main()

