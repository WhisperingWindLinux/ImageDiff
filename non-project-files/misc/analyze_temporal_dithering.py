import cv2
import numpy as np
import argparse
import os


# pip install opencv-python opencv-python-headless numpy

def analyze_temporal_dithering(input_video_path, output_video_path):
    # Open the input video
    cap = cv2.VideoCapture(input_video_path)
    if not cap.isOpened():
        print("Failed to open the video.")
        return

    # Get video parameters
    frame_width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    frame_height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    fps = int(cap.get(cv2.CAP_PROP_FPS))
    frame_count = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))

    # Initialize the output video writer
    fourcc = cv2.VideoWriter_fourcc(*'XVID')
    out = cv2.VideoWriter(output_video_path, fourcc, fps, (frame_width, frame_height), isColor=False)

    # Variable to store the previous frame
    prev_frame = None
    all_frames_identical = True  # Flag to check if all frames are identical

    for _ in range(frame_count):
        ret, frame = cap.read()
        if not ret:
            break

        # Convert the current frame to grayscale
        gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

        if prev_frame is None:
            # If this is the first frame, just store it as the previous frame
            prev_frame = gray_frame
            # Create a completely white frame (all pixels are identical)
            diff_frame = np.full_like(gray_frame, 255)
        else:
            # Calculate the difference between the current and previous frames
            diff_frame = cv2.absdiff(gray_frame, prev_frame)
            _, diff_frame_binary = cv2.threshold(diff_frame, 1, 255, cv2.THRESH_BINARY)

            # Invert colors: identical pixels are white, changed pixels are black
            result_frame = cv2.bitwise_not(diff_frame_binary)

            # Check if there are any changes in the current frame
            if np.any(diff_frame_binary > 0):
                all_frames_identical = False

            # Save the result to the output video
            out.write(result_frame)

            # Update the previous frame
            prev_frame = gray_frame

    # Release resources
    cap.release()
    out.release()

    # Output the analysis result
    if all_frames_identical:
        print("All frames in the video are identical.")
    else:
        print("Differences between frames were detected in the video.")


if __name__ == "__main__":
    # Set up argument parsing
    parser = argparse.ArgumentParser(description="Analyze temporal dithering in a video.")
    parser.add_argument("input_video", type=str, help="Path to the input video file.")

    # Parse arguments from the command line
    args = parser.parse_args()

    # Generate output video path with ".mp4" extension
    input_path = args.input_video
    input_dir, input_filename = os.path.split(input_path)
    input_name, _ = os.path.splitext(input_filename)

    output_filename = f"{input_name}_output.mov"
    output_path = os.path.join(input_dir, output_filename)

    # Call the function with the generated output path
    analyze_temporal_dithering(args.input_video, output_path)

    print(f"Output video saved to: {output_path}")
