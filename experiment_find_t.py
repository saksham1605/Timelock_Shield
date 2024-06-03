import subprocess
import time
def extract_and_average_t_values(t_value):
    x=t_value
    try:
        # Define the command to execute with the variable t_value
        command = f"./bin/vdf -t {t_value} -i bin/num.txt"
        
        # Initialize a list to store the values of T
        t_values = []
        
        # Run the command 5 times
        for _ in range(7):
            time.sleep(2)
            output = subprocess.check_output(command, shell=True).decode()
            
            # Find the line containing the value of T
            for line in output.split('\n'):
                if "T:" in line:
                    t_value = int(line.split(':')[1].strip())
                    t_values.append(t_value)
                    break
        
        # Calculate the average of T values
        average_t = sum(t_values) / len(t_values)
        
        # Print the values of T and the average
        print(f"Values of T for -t {x}:", t_values)
        print(f"Average of T for -t {t_value}:", average_t)
    
    except Exception as e:
        print(f"An error occurred: {e}")

# Call the function with different values of t_value
# extract_and_average_t_values(3)
# extract_and_average_t_values(12)
# extract_and_average_t_values(60)
# extract_and_average_t_values(180)
# extract_and_average_t_values(1200)
extract_and_average_t_values(2400)

