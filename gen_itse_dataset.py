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
    num_samples = 1000
    data_path = "./itse_dataset/test/"
    scene_path = "/home/alex/dev/projects/libhand-public/nyu_synth/scene_spec.yml"
    config_paths = [
        "closed-no-body-rotation.json",
        "flat-no-body-rotation.json",
        "halfopen-no-body-rotation.json",
        "open-no-body-rotation.json",
        "pinching-no-body-rotation.json",
        "point-no-body-rotation.json"
    ]

    # Create directory
    Path(data_path).mkdir(parents=True, exist_ok=True)
    Path(data_path + "depth/").mkdir(parents=True, exist_ok=True)

    f = open(os.path.join(data_path, "annotations.pkl"), "wb")
    pickle.dump(num_samples * len(config_paths), f)

    for i in trange(num_samples * len(config_paths)):
        if i % num_samples == 0:
            config_path_idx = int(i / num_samples)
            config_path = config_paths[config_path_idx]
            pose_gen = PoseGenerator.PoseGenerator(scene_path, config_path)
        sample = pose_gen.GetSample()
        filename = "depth/depth_{}.png".format(i)
        img = Image.fromarray(sample.depth)
        img.save(os.path.join(data_path, filename))
        pickle.dump(sample.joints, f)
    f.close()


if __name__ == "__main__":
    main()

