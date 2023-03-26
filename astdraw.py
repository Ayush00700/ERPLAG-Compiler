import graphviz

class Node:
    def __init__(self, value):
        self.value = value
        self.children = []

    def add_child(self, child):
        self.children.append(child)

def visualize_ast(root):
    dot = graphviz.Digraph()
    _traverse_ast(dot, root)
    return dot

def _traverse_ast(dot, node):
    dot.node(str(id(node)), label=str(node.value))
    for child in node.children:
        dot.edge(str(id(node)), str(id(child)))
        _traverse_ast(dot, child)

# Create a sample AST
root = Node("Program")
root.add_child(Node("VariableDeclaration"))
root.add_child(Node("FunctionDeclaration"))
root.children[0].add_child(Node("x"))
root.children[1].add_child(Node("add"))
root.children[1].children[0].add_child(Node("a"))
root.children[1].children[0].add_child(Node("b"))

# Visualize the AST
dot = visualize_ast(root)
dot.render("ast", format="png")
