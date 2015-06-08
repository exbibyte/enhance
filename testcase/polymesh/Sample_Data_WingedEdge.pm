vertices:
  - { id: 0, pos: [  0,  8,  0 ] }
  - { id: 1, pos: [ -8,  0,  0 ] }
  - { id: 2, pos: [  0,  0,  0 ] }
  - { id: 3, pos: [  8,  0,  0 ] }
edges:
  - { id: 0, vert_ids: [  0, 1 ] }
  - { id: 1, vert_ids: [  1, 2 ] }
  - { id: 2, vert_ids: [  2, 0 ] }
  - { id: 3, vert_ids: [  0, 3 ] }
  - { id: 4, vert_ids: [  3, 2 ] }
faces:
  - { id: 0, vert_ids: [ 0, 1, 2 ], dir_verts_ccw: 1, dir_norm_align_verts: 1 }
  - { id: 1, vert_ids: [ 2, 0, 3 ], dir_verts_ccw: 0, dir_norm_align_verts: 0 }
