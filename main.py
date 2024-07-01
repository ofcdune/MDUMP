import networkx as nx
import matplotlib.pyplot as plt


class Parser:

    def __init__(self, data: bytes):
        self.__data = data
        self.__index = 0
        self.__vars = {}
        self.__structs = {}

    def parse_data(self):

        while self.__index < len(self.__data):
            tbyte = self.__data[self.__index]
            self.__index += 1
            ptr = False

            match tbyte:
                case 0x01 | 0x02:
                    length = 1
                case 0x08:
                    length = 8
                case 0x09 | 0x0a | 0x10:
                    length = 8
                    ptr = True
                case _:
                    self.__index += 1
                    continue

            data, address = self.parse_line(self.__data[self.__index:self.__index + 9])
            self.__vars[address] = {"buffer": bytes([data]), "pointer": ptr, "type": "int"}

            for _ in range(length - 1):
                data, _ = self.parse_line(self.__data[self.__index:self.__index + 9])
                self.__vars[address]["buffer"] = self.__vars[address]["buffer"] + bytes([data])

        return self.__vars

    def parse_struct(self):

        address = b's'

        while self.__index < len(self.__data):
            tbyte = self.__data[self.__index]
            self.__index += 1
            ptr = False

            match tbyte:
                case 0x01:
                    self.__index += 1
                case 0x11:
                    ptr = True
                case _:
                    self.__index += 1
                    continue

            if ptr:
                buffer = b''
                for _ in range(8):
                    data = self.__data[self.__index]
                    self.__index += 1
                    buffer = buffer + bytes([data])

                self.__structs[address].append(buffer.hex())

                continue

            address = self.__data[self.__index:self.__index + 8].hex()
            self.__index += 8

            self.__structs[address] = []

        return self.__structs

    def parse_line(self, data_section) -> list[bytes]:
        x = [data_section[0], data_section[1:]]
        self.__index += 9
        return x


g = nx.DiGraph()

nullp = "00"*8
g.add_node(nullp, display_name="NULL")  # the null pointer


def vars_to_graph(vars: dict):
    for index, var in enumerate(vars):

        g.add_node(var, display_name=str(index))
        if vars[var]["pointer"]:
            g.add_edge(var, vars[var]["buffer"], display_name=str(index))


def struct_to_graph(structs: dict):

    for index, struct in enumerate(structs):
        g.add_node(struct, display_name=str(index))

        for pointer in structs[struct]:
            g.add_edge(struct, pointer)


def find_null_edge():
    if g.in_degree(nullp) == 0:
        g.remove_node(nullp)


file = open("test.mdump", "rb").read()

parser = Parser(file)
sparse = parser.parse_struct()

struct_to_graph(sparse)
find_null_edge()

# Get positions for the nodes
pos = nx.spectral_layout(g)

# Draw nodes and edges
nx.draw_networkx_nodes(g, pos, node_size=100)
nx.draw_networkx_edges(g, pos, width=1, arrows=True, arrowstyle='-|>', arrowsize=10)

# Draw node labels using the custom display names
custom_labels = nx.get_node_attributes(g, 'display_name')
nx.draw_networkx_labels(g, pos, labels=custom_labels, font_size=12)

plt.show()
