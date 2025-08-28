import numpy as np
import cv2

# width and height of the animation
WIDTH = 7
HEIGHT = 5

folder_name = input("Animation folder name: ")
num_frames = int(input("Number of frames: "))
fps = int(input("Frames per second: "))

with open(f"misc_python/image_output/{folder_name}.bmpani", "wb") as f:
    f.write(WIDTH.to_bytes(1, "big"))
    f.write(HEIGHT.to_bytes(1, "big"))
    f.write(num_frames.to_bytes(1, "big"))
    f.write(fps.to_bytes(1, "big"))
    for n in range(num_frames):
        cv_im = cv2.imread(f"misc_python/image_input/{folder_name}/{n}.png")
        np_im = np.asarray(cv_im)
        for i in range(WIDTH):
            for j in range(HEIGHT):
                f.write(int(np_im[j, i, 0]).to_bytes(1, "big"))
                f.write(int(np_im[j, i, 1]).to_bytes(1, "big"))
                f.write(int(np_im[j, i, 2]).to_bytes(1, "big"))

print(f"Animation \"{folder_name}.bmpani\" successfully generated")