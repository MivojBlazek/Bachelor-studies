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
    data_frames = []
    data_zip = zipfile.ZipFile(filename, 'r')
    
    for file in data_zip.namelist():
        if ds in file:
            file_opened = data_zip.open(file)
            df = pd.read_html(file_opened, encoding='cp1250')[0]
            df = df.loc[:, ~df.columns.str.contains('^Unnamed')]
            data_frames.append(df)

    data_zip.close()
    return pd.concat(data_frames, ignore_index=True)


# Ukol 2: zpracovani dat
def parse_data(df: pd.DataFrame, verbose: bool = False) -> pd.DataFrame:
    df2 = df.copy()

    df2['date'] = pd.to_datetime(df2['p2a'], format='%d.%m.%Y')

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
    # p4a je kraj (uzemni misto dopravni nehody)
    df2['region'] = df2['p4a'].map(region)

    df2.drop_duplicates(subset='p1')

    if verbose:
        print(f'new_size={df2.memory_usage(index=True, deep=True).sum() / (10 ** 6):.1f} MB')

    return df2

# Ukol 3: počty nehod v jednotlivých regionech podle stavu vozovky
def plot_state(df: pd.DataFrame, fig_location: str = None,
                    show_figure: bool = False):
    fig, axes = plt.subplots(2, 2, figsize=(13, 8), sharex=True)

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

    for i, state_name in enumerate(df['state'].dropna().unique()):
        ax = axes[i//2, i%2]

        state_data = df[df['state'] == state_name]
        state_counts = state_data.groupby('region')['region'].agg('count')
        
        ax.bar(state_counts.index, state_counts.values, width=0.8, bottom=0, color=colors[i])

        ax.set_title(f'Stav povrchu vozovky: {state_name}')
        if i//2 == 1:
            ax.set_xlabel('Kraj')
        if i%2 == 0:
            ax.set_ylabel('Počet nehod')
        ax.set_facecolor('cyan')
        ax.grid(axis='y', color='red', linewidth=1)
    
    fig.suptitle('Počet nehod dle povrchu vozovky')
    fig.tight_layout()

    if fig_location is not None:
        fig.savefig(fig_location)
    if show_figure:
        plt.show()


# Ukol4: alkohol a následky v krajích
def plot_alcohol(df: pd.DataFrame, df_consequences : pd.DataFrame, 
                 fig_location: str = None, show_figure: bool = False):
    pass

# Ukol 5: Druh nehody (srážky) v čase
def plot_type(df: pd.DataFrame, fig_location: str = None,
              show_figure: bool = False):
    pass


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
