import os
import sys
import cv2
import numpy as np
import tempfile
import matplotlib.pyplot as plt


def compare_gamma(image1, image2, name1, name2):
    hist1 = cv2.calcHist([image1], [0], None, [256], [0, 256])
    hist2 = cv2.calcHist([image2], [0], None, [256], [0, 256])

    hist1 = hist1 / hist1.sum()
    hist2 = hist2 / hist2.sum()

    plt.figure(dpi=300)
    plt.plot(hist1, label=name1)
    plt.plot(hist2, label=name2)
    plt.title("Gamma Curve Comparison")
    plt.legend()

    with tempfile.NamedTemporaryFile(suffix=".png", delete=False) as temp_file:
        temp_file_path = temp_file.name
        plt.savefig(temp_file_path)
        plt.close()
        image = cv2.imread(temp_file_path, cv2.IMREAD_COLOR)
        os.remove(temp_file_path)
        return image


def main():
    if len(sys.argv) != 3:
        print("Usage: script.py <name1> <name2>", file=sys.stderr)
        sys.exit(1)

    name1 = sys.argv[1]
    name2 = sys.argv[2]

    try:
        my_variable = os.getenv("Runner")
        if my_variable == "TwinPix":
            # Read the first image's size and data from stdin
            size1 = int.from_bytes(sys.stdin.buffer.read(4), byteorder='little')
            image1_data = sys.stdin.buffer.read(size1)
            # Read the second image's size and data from stdin
            size2 = int.from_bytes(sys.stdin.buffer.read(4), byteorder='little')
            image2_data = sys.stdin.buffer.read(size2)
            # Decode the images from the binary data
            image1 = cv2.imdecode(np.frombuffer(image1_data, np.uint8), cv2.IMREAD_GRAYSCALE)
            image2 = cv2.imdecode(np.frombuffer(image2_data, np.uint8), cv2.IMREAD_GRAYSCALE)
        else:
            image1 = cv2.imread(name1, cv2.IMREAD_GRAYSCALE)
            image2 = cv2.imread(name2, cv2.IMREAD_GRAYSCALE)

        if image1 is None or image2 is None:
            print("Failed to decode images", file=sys.stderr)
            sys.exit(1)

        if image1.shape != image2.shape:
            print("Images must have the same dimensions", file=sys.stderr)
            sys.exit(1)

        # Compare the images and get the result
        result_image = compare_gamma(image1, image2, name1, name2)

        # Encode the resulting image to PNG format
        _, encoded_image = cv2.imencode('.png', result_image)
        # Write the encoded image to stdout
        if my_variable == "TwinPix":
            sys.stdout.buffer.write(encoded_image.tobytes())
        else:
            cv2.imshow("Differences", result_image)
            cv2.waitKey(0)
            cv2.destroyAllWindows()

    except Exception as e:
        print(f"An error occurred: {e}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
