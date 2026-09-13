import matplotlib.pyplot as plt

benchmarks = ['456.hmmer', '429.mcf', '473.astar']
reductions = [5.23, 1.57, 1.20]

plt.figure(figsize=(7, 4.5))
bars = plt.bar(benchmarks, reductions, color='#1f77b4', width=0.5, edgecolor='black')

plt.title('Plot 2: LLC Miss-Rate Reduction over LRU (2MB, 16-way LLC)', fontsize=12, pad=12)
plt.xlabel('Benchmark', fontsize=11)
plt.ylabel('LLC Miss-Rate Reduction (%)', fontsize=11)
plt.grid(axis='y', linestyle='--', alpha=0.7)
plt.ylim(0, 6.0)

for bar in bars:
    yval = bar.get_height()
    plt.text(bar.get_x() + bar.get_width()/2.0, yval + 0.12, f'{yval:.2f}%', ha='center', va='bottom', fontweight='bold')

plt.tight_layout()
plt.savefig('plot2_miss_rate_reduction.png', dpi=300)
plt.show()
