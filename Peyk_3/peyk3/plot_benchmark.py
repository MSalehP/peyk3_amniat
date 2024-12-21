import pandas as pd
import matplotlib.pyplot as plt

# Load data
data = pd.read_csv('times.csv')

# Create a BoxPlot
plt.boxplot([data['AES_Time'], data['RSA_Time']], tick_labels=['AES', 'RSA'])
plt.ylabel('Time (milliseconds)')
plt.title('AES vs RSA Performance Comparison')

# Save the plot to a file
plt.savefig('benchmark_plot.png')

# If you want to display it, you can use plt.show() (optional in non-interactive environments)
# plt.show()

