import os
import sys

if os.path.abspath("build/src") not in sys.path:
    sys.path.append(os.path.abspath("build/src"))

import matplotlib.pyplot as plt
import numpy as np
import PoseGenerator


def main():
    scene_path = "/home/alex/dev/projects/libhand-public/nyu_synth/scene_spec.yml"
    config_path = "all-no-body-rotation.json"
    pose_gen = PoseGenerator.PoseGenerator(scene_path, config_path)
    sample = pose_gen.GetSample()

    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.imshow(sample.depth)
    plt.show()


if __name__ == "__main__":
    main()

