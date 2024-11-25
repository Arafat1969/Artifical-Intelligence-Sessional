import os
import pandas as pd

# Absolute path to the directory containing TSP output files
tsp_dir = r"C:\Desktop\CSE 318\TSP\Output"

# List of TSP output files in the directory
tsp_files = [
    os.path.join(tsp_dir, "a280_out.tsp"),
    os.path.join(tsp_dir, "berlin52_out.tsp"),
    os.path.join(tsp_dir, "bier127_out.tsp"),
    os.path.join(tsp_dir, "ch130_out.tsp"),
    os.path.join(tsp_dir, "ch150_out.tsp"),
    os.path.join(tsp_dir, "eil101_out.tsp"),
    os.path.join(tsp_dir, "eil51_out.tsp"),
    os.path.join(tsp_dir, "eil76_out.tsp"),
    os.path.join(tsp_dir, "kroA100_out.tsp"),
    os.path.join(tsp_dir, "kroB100_out.tsp"),
    os.path.join(tsp_dir, "kroC100_out.tsp"),
    os.path.join(tsp_dir, "kroD100_out.tsp"),
    os.path.join(tsp_dir, "kroE100_out.tsp"),
    os.path.join(tsp_dir, "lin105_out.tsp"),
    os.path.join(tsp_dir, "lin318_out.tsp"),
    os.path.join(tsp_dir, "pr124_out.tsp"),
    os.path.join(tsp_dir, "pr144_out.tsp"),
    os.path.join(tsp_dir, "pr76_out.tsp"),
    os.path.join(tsp_dir, "rat195_out.tsp"),
    os.path.join(tsp_dir, "rat99_out.tsp"),
    os.path.join(tsp_dir, "st70_out.tsp"),
]

# Function to parse a TSP file and extract metrics
def parse_tsp_file(file_path):
    data = {}
    with open(file_path, 'r') as f:
        for line in f:
            if ":" in line:
                try:
                    approach, value = line.strip().split(":")
                    # Attempt to convert the value to float to verify it's a valid metric
                    data[approach.strip()] = float(value.strip())
                except ValueError:
                    # Skip lines that do not have a valid float value
                    print(f"Skipping invalid line in {file_path}: {line.strip()}")
            else:
                # Skip lines that do not contain valid metrics
                print(f"Skipping non-metric line in {file_path}: {line.strip()}")
    return data

# List to store all parsed data for the CSV file
all_data = []

# Collect data from all TSP files
for tsp_file in tsp_files:
    if not os.path.exists(tsp_file):
        print(f"File not found: {tsp_file}")
        continue

    try:
        tsp_data = parse_tsp_file(tsp_file)
        if tsp_data:
            tsp_data['File'] = os.path.basename(tsp_file)
            all_data.append(tsp_data)
        else:
            print(f"No valid data found in {tsp_file}, skipping.")
    except Exception as e:
        print(f"Error reading {tsp_file}: {e}")
        continue

# Create a DataFrame from all collected data
df = pd.DataFrame(all_data)

# Reorder columns so that 'File' comes first
columns = ['File'] + [col for col in df.columns if col != 'File']
df = df[columns]

# Save the DataFrame to a CSV file
csv_output_path = os.path.join(tsp_dir, "tsp_results_summary.csv")
df.to_csv(csv_output_path, index=False)

print(f"CSV file generated: {csv_output_path}")
