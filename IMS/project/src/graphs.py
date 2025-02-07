import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter

def load_data(filename: str) -> pd.DataFrame:
    data = pd.read_csv(filename, sep='\t', header=0)
    x = np.arange(len(data))
    y = data['Prijmy']
    coeffs = np.polyfit(x, y, deg=3)
    poly_fit = np.polyval(coeffs, x)
    start_offset = 0.3 * (y.iloc[0] - poly_fit[0])
    poly_fit += start_offset
    data['Prijmy_prolozeni'] = poly_fit 
    return data

def create_graph(data: pd.DataFrame, output: str = None):
    fig, axes = plt.subplots(2, figsize=(10, 8))
    ax = axes.flatten()

    # Data displaying
    ax[0].plot(data['Marketing'], data['Prijmy'], label='Příjmy', color='green', alpha=0.6)
    ax[0].plot(data['Marketing'], data['Prijmy_prolozeni'], label='Proložení grafu příjmů', color='#55AA55', linestyle='--', alpha=0.6)
    ax[0].plot(data['Marketing'], data['Vydaje'], label='Výdaje', color='red', alpha=0.6)

    ax[1].plot(data['Marketing'], data['Sucho'], label='Suché počasí', color='green')
    ax[1].plot(data['Marketing'], data['Dest'], label='Deštivé počasí', color='blue')

    # Graphical improvement and titles
    ax[0].set_title('Příjmy a výdaje v závislosti na marketingu')
    ax[0].set_xlabel('Velikost marketingu')
    ax[0].set_xlim(min(data['Marketing']), max(data['Marketing']))
    ax[0].set_xticks(np.arange(min(data['Marketing']), max(data['Marketing']), 50))
    ax[0].set_ylabel('Příjmy a výdaje v dolarech')
    ax[0].set_ylim(min(data['Prijmy']) * 0.95, max(data['Vydaje']) * 1.05)
    ax[0].legend()
    ax[0].grid(color='#DDDDDD')

    def billions(x, pos):
        return f'{int(x * 1e-9)} mld'
    ax[0].yaxis.set_major_formatter(FuncFormatter(billions))

    ax[1].set_title('Střední hodnota příchodu návstěvníků podle velikosti marketingu')
    ax[1].set_xlabel('Velikost marketingu')
    ax[1].set_xlim(min(data['Marketing']), max(data['Marketing']))
    ax[1].set_xticks(np.arange(min(data['Marketing']), max(data['Marketing']), 50))
    ax[1].set_ylabel('Střední hodnota příchodu návstěvníků')
    ax[1].set_ylim(0, max(data['Dest']) + 1)
    ax[1].set_yticks(np.arange(0, max(data['Dest']) + 1, 1))
    ax[1].legend()
    ax[1].grid(color='#DDDDDD')

    def minutes(x, pos):
        return f'{int(x)} min'
    ax[1].yaxis.set_major_formatter(FuncFormatter(minutes))

    fig.suptitle('Příjmy, výdaje a střední hodnoty příchodu návstěvníků za období 3 let')
    fig.tight_layout()

    if output is not None:
        fig.savefig(output)
    plt.show()


if __name__ == '__main__':
    file = 'data.txt'
    output_filename = 'graph.png'
    data = load_data(file)
    # create_graph(data)
    create_graph(data, output_filename)
