#!/usr/env python3

import h5py as hd

f = hd.File ( 'blaa.mat', 'r' )

print ( list( f.keys() ) )


# Access the data without loading the whole file
ds = f['M_PI']
print ( ds.shape )
print ( type(ds) )

# Access an inset of the loaded data
nA = ds[500:2500, 500]
print ( nA.shape )
print ( type(nA) )
