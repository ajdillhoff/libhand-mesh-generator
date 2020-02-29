import os
import sys
import pickle

if os.path.abspath("build/src") not in sys.path:
    sys.path.append(os.path.abspath("build/src"))

from tqdm import trange
from PIL import Image
import matplotlib.pyplot as plt
import numpy as np
import PoseGenerator


def main():
    num_samples = 2000000
    data_path = "./dataset/train/"
    scene_path = "/home/alex/dev/projects/libhand-public/nyu_synth/scene_spec.yml"
    config_path = "config.json"
    pose_gen = PoseGenerator.PoseGenerator(scene_path, config_path)

    f = open(os.path.join(data_path, "annotations.pkl"), "wb")
    pickle.dump(num_samples, f)
    for i in trange(num_samples):
        sample = pose_gen.GetSample()
        filename = "depth/depth_{}.png".format(i)
        img = Image.fromarray(sample.depth)
        img.save(os.path.join(data_path, filename))
        pickle.dump(sample.joints, f)
    f.close()


if __name__ == "__main__":
    main()
