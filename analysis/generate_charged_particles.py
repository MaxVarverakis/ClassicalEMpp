import random

num_particles = 30

for i in range(num_particles):
    charge = 0
    while charge == 0:
        charge = random.uniform(-1.0, 1.0)
    
    mass = random.uniform(5.0, 50.0)
    x = random.uniform(-5.0, 5.0)
    y = random.uniform(-5.0, 5.0)
    vx = random.uniform(-1.0, 1.0)
    vy = random.uniform(-1.0, 1.0)
    print('{' + f'"charge": {charge:.3f}, "mass": {mass:.3f}, "x": {x:.3f}, "y": {y:.3f}, "vx": {vx:.3f}, "vy": {vy:.3f}, "vz": 0.0' + '},')
