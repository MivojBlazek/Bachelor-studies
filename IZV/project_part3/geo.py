#!/usr/bin/python3.10
# coding=utf-8
# %%%
import pandas as pd
import geopandas
import matplotlib.pyplot as plt
import contextily
import sklearn.cluster
import numpy as np

def make_geo(df_accidents: pd.DataFrame, df_locations: pd.DataFrame) -> geopandas.GeoDataFrame:
    """Function creates GeoDataFrame from given DataFrames according to positions.

    Args:
        df_accidents (pd.DataFrame): DataFrame of accidents.
        df_locations (pd.DataFrame): DataFrame of locations of these accidents.

    Returns:
        geopandas.GeoDataFrame: GeoDataFrame of merged DataFrames from function parameters
                                with correct data of accident locations.
    """
    df_merged = pd.merge(df_accidents, df_locations, on='p1', how='inner')
    df_merged = df_merged.dropna(subset=['d', 'e'])

    # Remove records with d=0 and e=0
    df_merged = df_merged[(df_merged['d'] != 0) & (df_merged['e'] != 0)]

    # Swap d and e values in records where d value is below e value
    swap_need = df_merged['d'] < df_merged['e']
    df_merged.loc[swap_need, ['d', 'e']] = df_merged.loc[swap_need, ['e', 'd']].values

    gdf = geopandas.GeoDataFrame(df_merged,
                                 geometry=geopandas.points_from_xy(df_merged['d'], df_merged['e']),
                                 crs='EPSG:5514')
    return gdf


def plot_geo(gdf: geopandas.GeoDataFrame, fig_location: str = None,
             show_figure: bool = False):
    """Function creates a graph of accidents with alcohol in specific
    region (VYS) in 2 specific months (March and July).

    Args:
        gdf (geopandas.GeoDataFrame): GeoDataFrame of geographic accidents data.
        fig_location (str): If set, figure is saved as image to file with this name. Defaults to None.
        show_figure (bool): Determines if figure is displayed. Defaults to False.
    """
    fig, axes = plt.subplots(1, 2, figsize=(11, 6))
    ax = axes.flatten()

    # Web map projection
    gdf = gdf.to_crs('EPSG:3857')

    # Change type of date from string to datetime
    gdf['date'] = pd.to_datetime(gdf['date'])

    # Filter only alcohol with region 'VYS'
    gdf_region = gdf[
        (gdf['region'] == 'VYS')
        & (gdf['p11'] >= 4)
    ]

    gdf_region[gdf_region['date'].dt.month == 3].plot(ax=ax[0], markersize=5, color='red')
    gdf_region[gdf_region['date'].dt.month == 7].plot(ax=ax[1], markersize=5, color='red')

    # Add map to background
    contextily.add_basemap(ax[0],
                           crs=gdf_region.crs.to_string(),
                           source=contextily.providers.OpenStreetMap.Mapnik,
                           alpha=0.9)
    contextily.add_basemap(ax[1],
                           crs=gdf_region.crs.to_string(),
                           source=contextily.providers.OpenStreetMap.Mapnik,
                           alpha=0.9)

    # Graphical improvements
    ax[0].axis('off')
    ax[0].set_title('Kraj Vysočina - pod vlivem alkoholu (Březen)')
    ax[0].set_aspect('equal')
    ax[1].axis('off')
    ax[1].set_title('Kraj Vysočina - pod vlivem alkoholu (Červenec)')
    ax[1].set_aspect('equal')
    fig.tight_layout()

    if fig_location is not None:
        fig.savefig(fig_location)
    if show_figure:
        plt.show()


def plot_cluster(gdf: geopandas.GeoDataFrame, fig_location: str = None,
                 show_figure: bool = False):
    """Function creates a graph of accidents with animals in specific
    region (VYS). It creates 5 clusters with number of accidents in them.

    Args:
        gdf (geopandas.GeoDataFrame): GeoDataFrame of geographic accidents data.
        fig_location (str): If set, figure is saved as image to file with this name. Defaults to None.
        show_figure (bool): Determines if figure is displayed. Defaults to False.
    """
    fig, ax = plt.subplots(1, 1, figsize=(8, 9))

    # Web map projection
    gdf = gdf.to_crs('EPSG:3857')

    # Filter only region 'VYS'
    gdf_region = gdf[
        (gdf['region'] == 'VYS')
        & (gdf['p10'] == 4) # animal
    ].copy()

    district_cities = {
        'Jihlava': (1733800, 6341900),
        'Žďár nad Sázavou': (1774800, 6371200),
        'Třebíč': (1767400, 6312000),
        'Pelhřimov': (1695000, 6349300),
        'Havlíčkův Brod': (1733800, 6377000),
    }
    initial_centers = np.array(list(district_cities.values()))

    #! If we want to see district cities that are used as initials centers of clusters
    # for (x, y) in district_cities.values():
    #     ax.scatter(x, y, color='blue', s=10)

    # Clusters preparing (n_clusters=5 because there are 5 districts in Vysočina region)
    coordinates = np.array(list(zip(gdf_region.geometry.x, gdf_region.geometry.y)))
    kmeans = sklearn.cluster.KMeans(n_clusters=5, init=initial_centers, n_init=1).fit(coordinates)
    gdf_region['cluster'] = kmeans.labels_

    # Count points in each cluster
    cluster_sizes = gdf_region['cluster'].value_counts().sort_index()
    norm = plt.Normalize(cluster_sizes.min(), cluster_sizes.max())
    cmap = plt.cm.viridis

    # Clusters displaying
    for cluster in cluster_sizes.index:
        cluster_points = gdf_region[gdf_region['cluster'] == cluster]
        cluster_area = cluster_points.geometry.union_all().convex_hull
        geopandas.GeoSeries([cluster_area]).plot(ax=ax, color=cmap(norm(cluster_sizes[cluster])), alpha=0.4)

    # Accidents displaying
    gdf_region.plot(ax=ax, markersize=5, color='red', alpha=0.5)

    # Add map to background
    contextily.add_basemap(ax,
                           crs=gdf_region.crs.to_string(),
                           source=contextily.providers.OpenStreetMap.Mapnik,
                           alpha=0.9)

    # Bar with color scale
    sm = plt.cm.ScalarMappable(cmap=cmap, norm=norm)
    sm.set_array([])
    color_bar = fig.colorbar(sm, ax=ax, orientation='horizontal',  pad=0.04, shrink=0.9)
    color_bar.set_label('Počet nehod v označeném úseku')

    # Graphical improvements
    ax.axis('off')
    ax.set_title('Nehody v kraji Vysočina zaviněné lesní zvěří nebo domácím zvířetem')
    ax.set_aspect('equal')
    fig.subplots_adjust(bottom=0.05, hspace=0.1, wspace=0.1)
    fig.tight_layout()

    if fig_location is not None:
        fig.savefig(fig_location)
    if show_figure:
        plt.show()


if __name__ == "__main__":
    # zde muzete delat libovolne modifikace
    df_accidents = pd.read_pickle("accidents.pkl.gz")
    df_locations = pd.read_pickle("locations.pkl.gz")
    gdf = make_geo(df_accidents, df_locations)

    plot_geo(gdf, "geo1.png", True)
    plot_cluster(gdf, "geo2.png", True)

    # testovani splneni zadani
    import os
    assert os.path.exists("geo1.png")
    assert os.path.exists("geo2.png")