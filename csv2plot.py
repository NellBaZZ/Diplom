import pandas
import matplotlib
import argparse

params = argparse.ArgumentParser()
params.add_argument('csv', type=argparse.FileType('r', encoding='utf-8'))
params = params.parse_args()

pd = pandas.read_csv(params.csv, index_col=0)
pd.plot(legend=False, xlabel="Размер, число слов по 32 бит", ylabel="Время, мс")
matplotlib.pyplot.show()