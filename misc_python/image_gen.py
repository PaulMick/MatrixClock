import numpy as np
import cv2

# width and height of the image
WIDTH = 7
HEIGHT = 5

fname = input("Image name (exclude .png extension): ")
cv_im = cv2.imread(f"misc_python/image_input/{fname}.png")
np_im = np.asarray(cv_im)

with open(f"misc_python/image_output/{fname}.bmpimg", "wb") as f:
    f.write(WIDTH.to_bytes(1, "big"))
    f.write(HEIGHT.to_bytes(1, "big"))
    for i in range(WIDTH):
        for j in range(HEIGHT):
            f.write(int(np_im[j, i, 0]).to_bytes(1, "big"))
            f.write(int(np_im[j, i, 1]).to_bytes(1, "big"))
            f.write(int(np_im[j, i, 2]).to_bytes(1, "big"))

print(f"Image \"{fname}.bmpimg\" successfully generated")