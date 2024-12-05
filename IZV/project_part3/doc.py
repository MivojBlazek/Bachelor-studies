import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns


def load_data() -> pd.DataFrame:
    """Function loads data from pickle files to one DataFrame.

    Returns:
        pd.DataFrame: DataFrame with merged data.
    """
    df_accidents = pd.read_pickle('accidents.pkl.gz')
    df_pedestrians = pd.read_pickle('pedestrians.pkl.gz')
    
    # Merge DataFrames into 1
    df = pd.merge(df_accidents, df_pedestrians, on='p1', how='inner')
    return df


def print_additional_info(contingency_table: pd.DataFrame):
    """Function calculates and prints additional information about graph.

    Args:
        contingency_table (pd.DataFrame): Contingency table of all information from graph.

    Returns:
        None:
    """
    with_reflex_sum = contingency_table.loc['Ano'].sum()
    without_reflex_sum = contingency_table.loc['Ne'].sum()
    multiple_of_accidents = without_reflex_sum / with_reflex_sum

    injury_values = {
        'Usmrcení': 3,
        'Těžké zranění': 2,
        'Lehké zranění': 1,
        'Bez zranění': 0
    }
    total_injury_with_reflex = sum(contingency_table.loc['Ano', key] * value for key, value in injury_values.items())
    total_injury_without_reflex = sum(contingency_table.loc['Ne', key] * value for key, value in injury_values.items())
    average_injury_with_reflex = total_injury_with_reflex / with_reflex_sum
    average_injury_without_reflex = total_injury_without_reflex / without_reflex_sum

    print(f'Celkový počet nehod s reflexními prvky: {with_reflex_sum}')
    print(f'Celkový počet nehod bez reflexních prvků: {without_reflex_sum}')
    print(f'Násobek nehod bez reflexních prvků oproti s nimi: {multiple_of_accidents:.2f}krát více nehod')
    print(f'Průměrná závažnost zranění s reflexními prvky: {average_injury_with_reflex:.4f}')
    print(f'Průměrná závažnost zranění bez reflexních prvků: {average_injury_without_reflex:.4f}')


def plot_reflex(df: pd.DataFrame, fig_location: str = None, show_figure: bool = False):
    """Function creates graph of pedestrians injury type and checks if he wore reflex items.

    Args:
        df (pd.DataFrame): DataFrame with required data (accidents and pedestrians).
        fig_location (str): If set, figure is saved as image to file with this name. Defaults to None.
        show_figure (bool): Determines if figure is displayed. Defaults to False.

    Returns:
        None:
    """
    fig, ax = plt.subplots(1, 1, figsize=(8, 8))
    df2 = df.copy()

    # Create column injury according to injury type in p33g
    injury = {
        1: 'Usmrcení',
        2: 'Těžké zranění',
        3: 'Lehké zranění',
        4: 'Bez zranění',
    }
    df2['injury'] = df2['p33g'].map(injury)

    # Create column reflex according to pedestrian reflex items in p29a
    reflex = {
        1: 'Ano',
        2: 'Ne',
    }
    df2['reflex'] = df2['p29a'].map(reflex)

    # Filter only accidents with pedestrians and with reduced visibility
    data = df2[
        (df2['p6'] == 4)
        & (df2['p19'] >= 2)
    ]

    # Create contingency table from reflex and injury
    print('Kontingenční tabulka:\n')
    contingency_table = pd.crosstab(index=data['reflex'],
                                    columns=data['injury'],
                                    rownames=['Přítomnost reflexních prvků'],
                                    colnames=['Zranění'])
    contingency_table_latex = contingency_table.to_latex(
        caption='Kontingenční tabulka následků nehody s chodcem s reflexními prvky a bez nich',
        label='tab:reflexni_prvky',
        index_names=True,
        header=True,
        bold_rows=True,
        column_format='|l|c|c|c|c|',
    )
    print(contingency_table_latex)

    # Printing calculated information
    print_additional_info(contingency_table)

    # Print countplot 
    sns.countplot(ax=ax, data=data,  x='reflex', hue='injury', zorder=10)

    # Graphical improvements
    ax.set_title('Následky nehody s chodcem s reflexními prvky a bez nich')
    ax.set_xlabel('Přítomnost reflexních prvků u chodce')
    ax.set_ylabel('Počet nehod')
    ax.set_yticks(np.arange(0, 1101, 100))
    ax.grid(axis='y', color='lightgray', zorder=0)
    ax.legend(title='Zranění')
    for bar in ax.patches:
        x = bar.get_x() + bar.get_width() / 2
        y = bar.get_height() + 0.05
        ax.text(x, y, str(int(bar.get_height())), ha='center', va='bottom', fontsize=9)

    if fig_location is not None:
        fig.savefig(fig_location)
    if show_figure:
        plt.show()


if __name__ == '__main__':
    df = load_data()
    plot_reflex(df, 'fig.png')
