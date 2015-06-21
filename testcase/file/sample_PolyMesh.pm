vertices:
   -{ id: 0, vec: [  0,  8,  0 ] }
   -{ id: 1, vec: [ -8,  0,  0 ] }
   -{ id: 2, vec: [  0,  0,  0 ] }
   -{ id: 3, vec: [  8,  0,  0 ] }
normals:
   -{ id: 0, vec: [  0,  0,  1 ] }
   -{ id: 1, vec: [  0,  0,  1 ] }
   -{ id: 2, vec: [  0,  0,  1 ] }
   -{ id: 3, vec: [  0,  0,  1 ] }
edges:
   -{ id: 0, vert_ids: [  0, 1 ] }
   -{ id: 1, vert_ids: [  1, 2 ] }
   -{ id: 2, vert_ids: [  2, 0 ] }
   -{ id: 3, vert_ids: [  0, 3 ] }
   -{ id: 4, vert_ids: [  3, 2 ] }
faces:
   -{ id: 0, vert_ids: [ 0, 1, 2 ], dir_verts_ccw: 1, dir_norm_align_verts: 1 }
   -{ id: 1, vert_ids: [ 2, 0, 3 ], dir_verts_ccw: 0, dir_norm_align_verts: 0 }
txsrcs:
   -{ id: 0, path: xyz.texturefile }
   -{ id: 1, path: abc.texturefile }
txcoords:
   -{ id: 0, vert_id: 0, ts_id: 0, coord: [ 0.5, 1.0 ] } 
   -{ id: 1, vert_id: 1, ts_id: 0, coord: [ 0.0, 0.0 ] }
   -{ id: 2, vert_id: 2, ts_id: 0, coord: [ 0.5, 0.0 ] }
   -{ id: 3, vert_id: 3, ts_id: 0, coord: [ 1.0, 0.0 ] }
