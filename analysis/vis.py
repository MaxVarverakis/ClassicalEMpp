import time
import json
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import matplotlib.animation as animation
plt.rcParams['text.usetex'] = True
plt.rcParams['font.size'] = 16

def readData(path, field):
    return pd.read_csv(path, header=None, names=['x', 'y', field, 'u', 'v'])

def vecPlot(data, cmap='RdBu', numDraw=1, scale=2, vmin=None, vmax=None, show=True):
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
    if show:
        plt.show()

def colorPlot(data, cmap='RdBu', vmin=None, vmax=None, show=True):
    X,Y = np.meshgrid(data['x'].unique(), data['y'].unique())
    C = data['E'].to_numpy().reshape(data['x'].unique().size, data['y'].unique().size).T
    plt.pcolormesh(X, Y, C, cmap=cmap, vmin=vmin, vmax=vmax)
    cb = plt.colorbar(extend='both')
    cb.set_label(r'$E$')
    plt.xlabel(r'$x$')
    plt.ylabel(r'$y$')
    plt.xlim([data['x'].min(), data['x'].max()])
    plt.ylim([data['y'].min(), data['y'].max()])
    if show:
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

def updatefig(frame, fname, field, clim, vec):
    data = readData(f'./outputs/{fname}{frame}.txt', field=field)
    plt.clf()
    if vec:
        vecPlot(data, vmin=-clim, vmax=clim, numDraw=6, scale=2, cmap='RdBu', show=False)
    else:
        colorPlot(data, vmin=-clim, vmax=clim, show=False)
    plt.title(f'Frame: {frame}')

if __name__ == '__main__':
    st = time.time()

    field = 'E'
    fname = 'dynamics_test/output_'

    inputs = json.load(open('./inputs/dynamics_test.json', 'r'))

    # for iteration in range(inputs["numSteps"]):
    # iteration = 0
    # data = readData(f'./outputs/{fname}{iteration}.txt', field=field)

    # magneticFieldPlot(data, numDraw=5, scale=4, vmax=.08)

    # clim=.1
    # vecPlot(data, vmin=-clim, vmax=clim, numDraw=6, scale=2, cmap='RdBu')

    # clim=1
    # colorPlot(data, vmin=-clim, vmax=clim)

    animation_time = 10 # s (= 50 fps for 500 frames)

    fig = plt.figure()
    anim = animation.FuncAnimation(fig, updatefig, inputs["numSteps"], fargs=(fname, field, 10, False), blit=False)
    anim.save("/Users/max/ClassicalEM++/animations/dynamics_4_particles_omp.mp4", fps=inputs["numSteps"]/animation_time, dpi=500)
    plt.close()
    
    print(f'Elapsed time: {time.time() - st:.2f} seconds!')
