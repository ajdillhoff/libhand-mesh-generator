import os
import sys
import pickle
from pathlib import Path

if os.path.abspath("build/src") not in sys.path:
    sys.path.append(os.path.abspath("build/src"))

from tqdm import trange
from PIL import Image
import matplotlib.pyplot as plt
import numpy as np
import PoseGenerator


def main():
    num_samples = 10000
    data_path = "./nyu_synth/val/"
    scene_path = "/home/alex/dev/projects/libhand-public/nyu_synth/scene_spec.yml"
    pose_config_path = "configs/pose/train.json"
    shape_config_path = "configs/shape/train.json"
    pose_gen = PoseGenerator.PoseGenerator(scene_path, pose_config_path, shape_config_path)

    # Create directory
    Path(data_path).mkdir(parents=True, exist_ok=True)
    Path(data_path + "depth/").mkdir(parents=True, exist_ok=True)

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

