import os
import subprocess

# Example usage:
input_folder = 'datasets/test/images'
output_folder = 'datasets/test/images'
prog_name = "Cut+Aspect"
out_name="_aug4.jpg"
params = "95"

# Get the list of image files in the input folder
image_files = [f for f in os.listdir(input_folder) if f.endswith('.jpg') or f.endswith('.jpeg')]
i = 0
image_files.sort()
# Apply the ImageDSP function to each image
for file_name in image_files:
    # Construct the input and output paths
    input_image = os.path.join(input_folder, file_name)
    output_image = os.path.join(output_folder, "image" + str(i) + out_name)  # Convert i to a string using str()
    
    # Construct the command to execute
    command = [
        './ImageDSP/build/linux/build-ImageDSP-Replacement_for_Desktop_Qt_6_5_1_GCC_64bit-Debug/ImageDSP',
        '--prog-name', prog_name,
        '--in-img', input_image,
        '--out-img', output_image,
        '--params', params
    ]

    # Execute the command using subprocess
    subprocess.run(command)
    i += 1

print("Batch processing completed.")
