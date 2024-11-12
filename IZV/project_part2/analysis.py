#!/usr/bin/env python3.12
# coding=utf-8

from matplotlib import pyplot as plt
import pandas as pd
import seaborn as sns
import numpy as np
import zipfile

# muzete pridat libovolnou zakladni knihovnu ci knihovnu predstavenou na prednaskach
# dalsi knihovny pak na dotaz

# Ukol 1: nacteni dat ze ZIP souboru
def load_data(filename : str, ds : str) -> pd.DataFrame:
    """This function loads dataframes from files to 1 dataframe.
    Data should be in html format.

    Args:
        filename (str): Zipped directory containing dataframes.
        ds (str): Name of the files without prefix and suffix.

    Returns:
        pd.DataFrame: Concatenated dataframe from specified files.
    """
    data_frames = []
    data_zip = zipfile.ZipFile(filename, 'r')

    for file in data_zip.namelist():
        if ds in file:
            file_opened = data_zip.open(file)

            # Read html content
            df = pd.read_html(file_opened, encoding='cp1250')[0]

            # Remove unnamed columns
            df = df.loc[:, ~df.columns.str.contains('^Unnamed')]
            data_frames.append(df)

    data_zip.close()

    # Concatenate all dataframes together (file = dataframe)
    return pd.concat(data_frames, ignore_index=True)


# Ukol 2: zpracovani dat
def parse_data(df: pd.DataFrame, verbose: bool = False) -> pd.DataFrame:
    """Function create a new dataframe from the given one and added column.
    Function can print memory usage information.

    Args:
        df (pd.DataFrame): Dataframe with required data.
        verbose (bool): Memory usage of new dataframe is printed after enabling. Defaults to False.

    Returns:
        pd.DataFrame: A new dataframe with column region and removed duplicates.
    """
    df2 = df.copy()

    # Create column 'date' from 'p2a'
    df2['date'] = pd.to_datetime(df2['p2a'], format='%d.%m.%Y')

    # Create column 'region' from 'p4a'
    region = {
        0: "PHA",
        1: "STC",
        2: "JHC",
        3: "PLK",
        4: "ULK",
        5: "HKK",
        6: "JHM",
        7: "MSK",
        14: "OLK",
        15: "ZLK",
        16: "VYS",
        17: "PAK",
        18: "LBK",
        19: "KVK",
    }
    df2['region'] = df2['p4a'].map(region)

    # Remove duplicate records according to 'p1' that is like ID
    df2.drop_duplicates(subset='p1')

    # Memory usage of entire dataframe printing
    if verbose:
        print(f'new_size={df2.memory_usage(index=True, deep=True).sum() / (10 ** 6):.1f} MB')

    return df2

# Ukol 3: počty nehod v jednotlivých regionech podle stavu vozovky
def plot_state(df: pd.DataFrame, fig_location: str = None,
                    show_figure: bool = False):
    """Function creates bar graphs of number of accidents in each region according to road state.

    Args:
        df (pd.DataFrame): Dataframe with required data.
        fig_location (str): If set, figure is saved as image to file with this name. Defaults to None.
        show_figure (bool): Determines if figure is displayed. Defaults to False.

    Returns:
        None:
    """
    fig, axes = plt.subplots(2, 2, figsize=(13, 8), sharex=True)

    # Create column 'state' from 'p16'
    state = {
        1: "suchý povrch",
        2: "suchý povrch",
        3: "mokrý povrch",
        4: "na vozovce je bláto",
        5: "na vozovce je náledí, ujetý sníh",
        6: "na vozovce je náledí, ujetý sníh",
    }
    df['state'] = df['p16'].map(state)

    colors = [
        'red',
        'green',
        'blue',
        'orange',
    ]

    # Create 4 subplots according to state values (there are 4 unique values)
    for i, state_name in enumerate(df['state'].dropna().unique()):
        ax = axes[i // 2, i % 2]

        # Countplot creates a bar graph with number of each state in regions.
        sns.countplot(data=df[df['state'] == state_name], x='region', ax=ax, zorder=3, color=colors[i])

        # Graphical improvement and titles
        ax.set_title(f'Stav povrchu vozovky: {state_name}')
        if i // 2 == 1:
            ax.set_xlabel('Kraj')
        if i % 2 == 0:
            ax.set_ylabel('Počet nehod')
        else:
            ax.set_ylabel('')
        ax.set_facecolor('#CCFFFF')
        ax.grid(axis='y', color='red', linewidth=1, zorder=0)

    fig.suptitle('Počet nehod dle povrchu vozovky')
    fig.tight_layout()

    if fig_location is not None:
        fig.savefig(fig_location)
    if show_figure:
        plt.show()


# Ukol4: alkohol a následky v krajích
def plot_alcohol(df: pd.DataFrame, df_consequences : pd.DataFrame, 
                 fig_location: str = None, show_figure: bool = False):
    """Function creates two-bar graphs of number of accidents in each region according to injured person.
    Displayed values are counted only when driver drank alcohol.

    Args:
        df (pd.DataFrame): Dataframe with required data.
        df_consequences (pd.DataFrame): Dataframe with required data about consequences.
        fig_location (str): If set, figure is saved as image to file with this name. Defaults to None.
        show_figure (bool): Determines if figure is displayed. Defaults to False.

    Returns:
        None:
    """
    # Merge dataframes together
    df_new = pd.merge(df, df_consequences, on='p1', how='inner')
    fig, axes = plt.subplots(2, 2, figsize=(13, 8), sharex=True)

    # Create new columns 'injury', 'injured_person' and 'alcohol'
    injury = {
        1: "Usmrcení",
        2: "Těžké zranění",
        3: "Lehké zranění",
        4: "Bez zranění",
    }
    df_new['injury'] = df_new['p59g'].map(injury)
    df_new['injured_person'] = df_new['p59a'].apply(lambda x: 'Řidič' if x == 1 else 'Spolujezdec')
    df_new['alcohol'] = df_new['p11'].apply(lambda x: 'Ano' if x >= 3 else 'Ne')

    # Create 4 subplots according to injury values (there are 4 unique values)
    for i, injury_name in enumerate(df_new['injury'].dropna().unique()):
        ax = axes[i // 2, i % 2]

        # Filter data for driver and passenger
        injury_data = df_new[
            (df_new['alcohol'] == 'Ano')
            & (df_new['injury'] == injury_name)
        ]

        # Graph displays a count of injured person type in each region
        sns.countplot(data=injury_data, x='region', hue='injured_person', ax=ax, zorder=3, legend=False)

        # Graphical improvement and titles
        ax.set_title(f'Následky nehody: {injury_name}')
        if i // 2 == 1:
            ax.set_xlabel('Kraj')
        ax.set_ylabel('Počet nehod pod vlivem')
        ax.set_facecolor('#CCFFFF')
        ax.grid(axis='y', color='white', linewidth=1, zorder=0)

    plt.legend(['Řidič', 'Spolujezdec'], loc='center left', bbox_to_anchor=(1.0, 1.05))
    fig.tight_layout()

    if fig_location is not None:
        fig.savefig(fig_location)
    if show_figure:
        plt.show()

# Ukol 5: Druh nehody (srážky) v čase
def plot_type(df: pd.DataFrame, fig_location: str = None,
              show_figure: bool = False):
    """Function creates line plot graphs of number of collisions in 4 randomly chosen regions.
    Graphs display collisions from 01/2023 to 10/2024 and each collision type as one line plot graph. 

    Args:
        df (pd.DataFrame): Dataframe with required data.
        fig_location (str): If set, figure is saved as image to file with this name. Defaults to None.
        show_figure (bool): Determines if figure is displayed. Defaults to False.

    Returns:
        None:
    """
    fig, axes = plt.subplots(2, 2, figsize=(13, 8))

    # Randomly choose 4 regions
    chosen_regions = np.random.choice(df['region'].dropna().unique(), 4, replace=False)
    df = df[df['region'].isin(chosen_regions)]

    # Create column 'collision' and filter only valid ones
    collision = {
        1: "s jedoucím nekolejovým vozidlem",
        2: "s vozidlem zaparkovaným",
        3: "s pevnou překážkou",
        4: "s chodcem",
        5: "s lesní zvěří",
        6: "s domácím zvířetem",
        7: "s vlakem",
        8: "s tramvají",
    }
    df = df[df['p6'].isin(collision.keys())]
    df['collision'] = df['p6'].map(collision)

    # Pivot table and remove multiindex
    df = df.pivot_table(columns='collision', index=['date', 'region'], aggfunc='size', fill_value=0)
    df = df.reset_index()
    df = df.set_index('date')

    # Group by region, downsample to each month and transform to stacked format
    df = df.groupby('region').resample('M').sum().stack().reset_index()

    # Filter date
    df = df[
        (df['date'] >= '2023-01-01')
        & (df['date'] <= '2024-10-01')
    ]

    colors = sns.color_palette('bright', len(collision))

    # Create 4 sublots according to 4 random regions
    for i, region_name in enumerate(chosen_regions):
        ax = axes[i // 2, i % 2]

        region_data = df[df['region'] == region_name]
        for j, collision_type in enumerate(collision.values()):
            # Check all collisions and create plot graph for each
            sns.lineplot(data=region_data[region_data['collision'] == collision_type],
                         x='date', y=0, ax=ax, legend=False, color=colors[j])

        # Graphical improvement and titles
        ax.set_title(f'Kraj: {region_name}')
        if i % 2 == 0:
            ax.set_ylabel('Počet nehod')
        else:
            ax.set_ylabel('')
        ax.set_xlabel('')
        plt.setp(ax.xaxis.get_majorticklabels(), rotation=45, ha='right', x=5)

    handles = [plt.Line2D([0], [0], color=colors[i]) for i in range(len(collision))]
    plt.legend(handles=handles, labels=collision.values(), loc='lower left', bbox_to_anchor=(1.0, 0.75), title='Druh nehody')
    fig.tight_layout()
    fig.subplots_adjust(hspace=0.4, wspace=0.15)

    if fig_location is not None:
        fig.savefig(fig_location)
    if show_figure:
        plt.show()


if __name__ == "__main__":
    # zde je ukazka pouziti, tuto cast muzete modifikovat podle libosti
    # skript nebude pri testovani pousten primo, ale budou volany konkreni
    # funkce.

    df = load_data("data_23_24.zip", "nehody")
    df_consequences = load_data("data_23_24.zip", "nasledky")
    df2 = parse_data(df, True)

    plot_state(df2, "01_state.png")
    plot_alcohol(df2, df_consequences, "02_alcohol.png", True)
    plot_type(df2, "03_type.png")

# Poznamka:
# pro to, abyste se vyhnuli castemu nacitani muzete vyuzit napr
# VS Code a oznaceni jako bunky (radek #%%% )
# Pak muzete data jednou nacist a dale ladit jednotlive funkce
# Pripadne si muzete vysledny dataframe ulozit nekam na disk (pro ladici
# ucely) a nacitat jej naparsovany z disku
