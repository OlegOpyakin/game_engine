# game_engine
Based on [series of videos](https://youtu.be/ih20l3pJoeU?si=wLg_fkEyXaHegQiT)


##What we can do?

Code can open any ```.obj``` file, it will be rendered in real time.

We can move in all dimensiones:
-> move forward - ```W```

-> move backward - ```S```

-> turn right - ```D```

-> turn left - ```A```

-> move up - ```E```

-> move down - ```Q```


##What is done?
1. Trasformation of objects from 3d to 2d
2. Rendered only those objects, which we can see
3. Vertex colour is based on global illumination
4. Vertexes clipping with screen edges


##Which external libraries used?
Only ```SDL3``` to display graphics