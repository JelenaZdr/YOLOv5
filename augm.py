import os
import shutil
import subprocess

# Example usage:
input_folder = 'datasets/valid/labels'
output_folder = 'datasets/valid/augmentation_labels'
prog_name = "Cutout"
out_name="_aug4.txt"
params = "95"

# Get the list of image files in the input folder
labels_files = [f for f in os.listdir(input_folder) if not f.endswith('labels.txt')]
i = 0
labels_files.sort()
# Apply the ImageDSP function to each image
for file_name in labels_files:
   shutil.copy(input_folder+"/"+file_name, output_folder+"/image"+str(i)+out_name)
   i=i+1
    
   

print("Batch processing completed.")
