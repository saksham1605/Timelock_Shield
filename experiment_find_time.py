import subprocess

def extract_and_average_values(t_value, x_value):
    try:
        # Define the command to execute with the variable t_value
        command = f"./bin/vdf -t 1 -i bin/num.txt -x {x_value}"
        t = t_value
        # Initialize lists to store the values of T and Puzzle Evaluation Time
        t_values = []
        puzzle_times = []
        
        # Run the command 5 times
        for _ in range(1):
            output = subprocess.check_output(command, shell=True).decode()
            
            # Find the line containing the value of T and Puzzle Evaluation Time
            for line in output.split('\n'):
                if "T:" in line:
                    t_value = int(line.split(':')[1].strip())
                    t_values.append(t_value)
                elif "Puzzle Evaluation Time:" in line:
                    time = float(line.split(':')[1].strip().split()[0])
                    puzzle_times.append(time)
        
        # Calculate the average of T values and Puzzle Evaluation Time
        average_t = sum(t_values) / len(t_values)
        average_time = sum(puzzle_times) / len(puzzle_times)
        
        # Print the values of T, Puzzle Evaluation Time, and their averages
        print(f"Values of T for -t {t} -x {x_value}:", t_values)
        print(f"Average of T for -t {t} -x {x_value}:", average_t)
        print(f"Puzzle Evaluation Times for -t {t} -x {x_value}:", puzzle_times)
        print(f"Average Puzzle Evaluation Time for -t {t} -x {x_value}:", average_time)
    
    except Exception as e:
        print(f"An error occurred: {e}")

# Call the function with different values of t_value
# extract_and_average_values(3,2700000)
# extract_and_average_values(12,10707428)
# extract_and_average_values(60,53125714)
extract_and_average_values(180,162334285)
extract_and_average_values(1200,1090971428)
# extract_and_average_values(2400,1315885714)
