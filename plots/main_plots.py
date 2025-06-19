from matplotlib.axes import Axes
import seaborn as sns
import pandas as pd
from matplotlib import pyplot as plt
import numpy as np

type_df = {
    "N":int,
    "EdgeExtractorName": pd.StringDtype, 
    "TimeInsertion": float, 
    "KruskalOptiPath": int, 
    "TimeKruskal": float,
}


df = pd.read_csv("./resultados/df.csv")
df = df.convert_dtypes()

df["Tiempo de Ejecución"] = df["TimeKruskal"] + df["TimeInsertion"]
df["Log_N"] = np.log2(df["N"])
df["KruskalOptiPath"] = df["KruskalOptiPath"].astype(bool)
df["Kruskal"] = df["TimeKruskal"]
df["Tiempo de Inserción"] = df["TimeInsertion"]

#for datapoint in data.iloc:
#    print(datapoint)

sns.set_theme(context= "paper", style="darkgrid", rc={"lines.linewidth": .5})

for column in ["Tiempo de Ejecución", "TimeInsertion", "TimeKruskal"]:
    g = sns.FacetGrid(df, 
                      col="EdgeExtractorName",  
                      row="KruskalOptiPath", 
                      height=4,
                      margin_titles=True)
    g.map_dataframe(sns.scatterplot, 
                    x="Log_N", 
                    y=column,
                    alpha=0.5)
    g.map_dataframe(sns.lineplot, 
                    x="Log_N", 
                    y=column, 
                    estimator=np.mean,
                    errorbar=("ci", 95))

    g.set_axis_labels(r"Número de Aristas $log_2(N)$", 
                      r"Tiempo de Ejecución Promedio [ms]")
    g.set_titles(col_template="{col_name} Edge Extractor", 
                 row_template="Optimization Path: {row_name}")



    plt.savefig(f"./resultados/{column}_all_graphs.png")

    for edge_extractor in ["ArraySort", "HeapMin"]:
        fig, ax = plt.subplots(figsize=(8, 6))
        sns.lineplot(
            data=df[df["EdgeExtractorName"] == edge_extractor], 
            x="Log_N", 
            y=column, 
            hue="KruskalOptiPath",
            ax=ax,
            estimator=np.mean,
            errorbar=('ci', 95),
            markers=True,
            markersize=10,
            legend=False)
        sns.scatterplot(
                data=df[df["EdgeExtractorName"] == edge_extractor], 
                x="Log_N", 
                y=column, 
                hue="KruskalOptiPath",
                ax=ax,
                alpha=0.6)
        ax.set_yscale("log")
        ax.set_xlabel(xlabel=r"Número de Aristas $log_2(N)$")
        ax.set_ylabel(ylabel=r"Tiempo de Ejecución Promedio $log(ms)$")
        ax.set_title(label=f"Comparación de Tiempos de Ejecución Promedio\ndel algoritmo de Kruskal utilizando el extractor de aristas {edge_extractor}")

        plt.savefig(f"./resultados/{column}_plot_{edge_extractor}.png")

    for opti_path  in [True, False]:
        fig, ax = plt.subplots(figsize=(8, 6))
        sns.lineplot(
            data=df[df["KruskalOptiPath"] == opti_path], 
            x="Log_N", 
            y=column, 
            hue="EdgeExtractorName",
            ax=ax,
            estimator=np.mean,
            errorbar=('ci', 95),
            markers=True,
            markersize=10,
            legend=False)
        sns.scatterplot(
                data=df[df["KruskalOptiPath"] == opti_path], 
                x="Log_N", 
                y=column, 
                hue="EdgeExtractorName",
                ax=ax,
                alpha=0.6)
        ax.set_yscale("log")
        ax.set_xlabel(xlabel=r"Número de Aristas $log_2(N)$")
        ax.set_ylabel(ylabel=r"Tiempo de Ejecución Promedio $log(ms)$")
        ax.set_title(label=f"Comparación de Tiempos de Ejecución Promedio\ndel algoritmo de Kruskal {"" if opti_path else "no "}utilizando la optimización del Union-Find")

        plt.savefig(f"./resultados/{column}_plot_opti_{opti_path}.png")
