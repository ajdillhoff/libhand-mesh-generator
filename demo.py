import os
import sys

if os.path.abspath("build/src") not in sys.path:
    sys.path.append(os.path.abspath("build/src"))

import matplotlib.pyplot as plt
import numpy as np
import PoseGenerator


def main():
    scene_path = "/home/alex/dev/projects/libhand-public/nyu_synth/scene_spec.yml"
    pose_config_path = "configs/pose/open-test-config.json"
    shape_config_path = "configs/shape/default.json"
    pose_gen = PoseGenerator.PoseGenerator(scene_path, pose_config_path, shape_config_path)
    s1 = pose_gen.GetSample()
    s2 = pose_gen.GetSample()
    s3 = pose_gen.GetSample()
    s4 = pose_gen.GetSample()

    fig = plt.figure()
    ax1 = fig.add_subplot(221)
    ax1.imshow(s1.depth)
    ax2 = fig.add_subplot(222)
    ax2.imshow(s2.depth)
    ax3 = fig.add_subplot(223)
    ax3.imshow(s3.depth)
    ax4 = fig.add_subplot(224)
    ax4.imshow(s4.depth)
    plt.show()


if __name__ == "__main__":
    main()

