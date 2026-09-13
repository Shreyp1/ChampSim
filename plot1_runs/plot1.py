import matplotlib.pyplot as plt


associativities = ['4-way', '8-way', '16-way']
lru_miss_rates = [29.54, 26.95, 25.68]
hawkeye_miss_rates = [30.68, 27.07, 24.33]


plt.figure(figsize=(7, 5))
plt.plot(associativities, lru_miss_rates, marker='o', color='#7f7f7f', label='LRU')
plt.plot(associativities, hawkeye_miss_rates, marker='s', color='#d62728', label='Hawkeye')


plt.title('Plot 1: LLC Miss Rate vs. Associativity (456.hmmer-191B)', fontsize=12, pad=12)
plt.xlabel('LLC Associativity (Fixed 2MB Capacity)', fontsize=11)
plt.ylabel('LLC Miss Rate (%)', fontsize=11)
plt.legend(fontsize=10)
plt.grid(True, linestyle='--', alpha=0.7)


for i, (lru, hawk) in enumerate(zip(lru_miss_rates, hawkeye_miss_rates)):
    plt.text(i, lru - 0.4, f'{lru}%', ha='center', va='top', fontsize=9, color='#333333')
    plt.text(i, hawk + 0.4, f'{hawk}%', ha='center', va='bottom', fontsize=9, color='#d62728')

plt.tight_layout()
plt.savefig('plot1_miss_rate_vs_associativity.png', dpi=300)
plt.show()
