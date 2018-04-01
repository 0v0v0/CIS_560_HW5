Questions: 

(1)Given a mesh with all of its half-edges created but none of its SYM pointers set, what is the minimum information needed to determine which half-edge should be the SYM of some other half-edge?

Answer: If the two halfedge->vertex are connected, then they are symmetric halfedges.


(2) Given the parallel nature of OpenGL shader execution, what do you suppose is a downside to using branching statements (e.g. if statements) in a shader program?

Answer: Since GPU will run all the code parallelly, it will NOT jump over the codes in "if" when the statement is not true. Therefore, branching saves no time. 



Functions And Bugs: 

In "half_edges": 
1. Classes of Vertex, Face, HalfEdges, Mesh
2. Half Edge Structure Composition

In "mygl":
3. The loading obj function
(location is hard-coded, after building, plz put the objs into the builded folder! )

Bugs: 

1. Qt UI unfinished. 
2. Currently no color, just a black cow! (See the scrrenshots)

This is really a loooooong project, sorry I delayed that for a year.....