import numpy as np
import matplotlib.pyplot as plt
plt.rcParams['text.usetex'] = True
plt.rcParams['font.size'] = 16
import pandas as pd

def readData(path, field):
    return pd.read_csv(path, header=None, names=['x', 'y', field, 'u', 'v'])

def vecPlot(data, cmap='RdBu', numDraw=1, scale=2, vmin=None, vmax=None):
    '''
    numDraw: number of vectors to draw (1 means draw all vectors, 5 means every 5th vector, and so on...)
    '''
    X,Y = np.meshgrid(data['x'].unique(), data['y'].unique())
    # reshape U,V,E to 2D arrays
    U = data['u'].to_numpy().reshape(data['x'].unique().size, data['y'].unique().size).T
    V = data['v'].to_numpy().reshape(data['x'].unique().size, data['y'].unique().size).T
    E = data['E'].to_numpy().reshape(data['x'].unique().size, data['y'].unique().size).T

    plt.quiver(X[::numDraw,::numDraw], Y[::numDraw,::numDraw], U[::numDraw,::numDraw], V[::numDraw,::numDraw], E[::numDraw,::numDraw], cmap=cmap, units='xy', scale=scale, scale_units='xy', clim=(vmin, vmax))
    cb = plt.colorbar(extend='both')
    cb.set_label(r'$E$')
    plt.xlabel(r'$x$')
    plt.ylabel(r'$y$')
    plt.xlim([data['x'].min(), data['x'].max()])
    plt.ylim([data['y'].min(), data['y'].max()])
    plt.show()

def colorPlot(data, cmap='RdBu', vmin=None, vmax=None):
    X,Y = np.meshgrid(data['x'].unique(), data['y'].unique())
    C = data['E'].to_numpy().reshape(data['x'].unique().size, data['y'].unique().size).T
    plt.pcolormesh(X, Y, C, cmap=cmap, vmin=vmin, vmax=vmax)
    cb = plt.colorbar(extend='both')
    cb.set_label(r'$E$')
    plt.xlabel(r'$x$')
    plt.ylabel(r'$y$')
    plt.xlim([data['x'].min(), data['x'].max()])
    plt.ylim([data['y'].min(), data['y'].max()])
    plt.show()

def magneticFieldPlot(data, cmap='Greys', numDraw=1, scale=2, vmin=0, vmax=None):
    '''
    numDraw: number of vectors to draw (1 means draw all vectors, 5 means every 5th vector, and so on...)
    '''
    X,Y = np.meshgrid(data['x'].unique(), data['y'].unique())
    # reshape U,V,E to 2D arrays
    B = data['B'].to_numpy().reshape(data['x'].unique().size, data['y'].unique().size).T
    U = data['u'].to_numpy().reshape(data['x'].unique().size, data['y'].unique().size).T
    V = data['v'].to_numpy().reshape(data['x'].unique().size, data['y'].unique().size).T

    plt.quiver(X[::numDraw,::numDraw], Y[::numDraw,::numDraw], U[::numDraw,::numDraw], V[::numDraw,::numDraw], B[::numDraw,::numDraw], units='xy', scale=scale, scale_units='xy', cmap=cmap, clim=(vmin, vmax))
    plt.xlabel(r'$x$')
    plt.ylabel(r'$y$')
    cb = plt.colorbar(extend='max')
    cb.set_label(r'$B$')
    plt.xlabel(r'$x$')
    plt.ylabel(r'$y$')
    plt.xlim([data['x'].min(), data['x'].max()])
    plt.ylim([data['y'].min(), data['y'].max()])
    plt.show()

def magneticFieldPlot2(data, numDraw=1, scale=2, maxLength=5):
    '''
    numDraw: number of vectors to draw (1 means draw all vectors, 5 means every 5th vector, and so on...)
    '''
    X,Y = np.meshgrid(data['x'].unique(), data['y'].unique())
    # reshape U,V,E to 2D arrays
    B = data['B'].to_numpy().reshape(data['x'].unique().size, data['y'].unique().size).T
    U = np.multiply(data['u'].to_numpy().reshape(data['x'].unique().size, data['y'].unique().size).T, B)
    V = np.multiply(data['v'].to_numpy().reshape(data['x'].unique().size, data['y'].unique().size).T, B)

    lengths = np.sqrt(U**2 + V**2)
    scale_lengths = np.minimum(1, maxLength/lengths)
    U *= scale_lengths
    V *= scale_lengths

    plt.quiver(X[::numDraw,::numDraw], Y[::numDraw,::numDraw], U[::numDraw,::numDraw], V[::numDraw,::numDraw], units='xy', scale=scale, scale_units='xy', color='k')
    plt.xlabel(r'$x$')
    plt.ylabel(r'$y$')
    plt.xlim([data['x'].min(), data['x'].max()])
    plt.ylim([data['y'].min(), data['y'].max()])
    plt.show()

if __name__ == '__main__':
    field = 'E'
    fname = 'negative_test'
    data = readData(f'./outputs/{fname}.txt', field=field)

    # magneticFieldPlot(data, numDraw=5, scale=4, vmax=.08)

    clim=.1
    vecPlot(data, vmin=-clim, vmax=clim, numDraw=6, scale=2, cmap='RdBu')

    # clim=1
    # colorPlot(data, vmin=-clim, vmax=clim)
