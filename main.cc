#include "Point.hh"
#include "Polygon.hh"
#include "Color.hh"

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <pngwriter.h>

using namespace std;

using vp = vector <Point>;


/* Checks whether the input has a wrong number of arguments. */
bool wrong_number(istringstream& iss) {
    string s;
    if (iss >> s) {
        cout << "error: command with wrong number of arguments";
        return true;
    }
    return false;
}


/* Checks whether the input has an undefined polygon as an argument. */
bool undef_id(map<string, Polygon>& Pols, const string& name) {
    bool error = Pols.count(name) == 0;
    if (error) cout << "error: undefined polygon identifier";
    return error;
}


/* Associates an identifier (name) with a convex polygon. */
void Polygon_def(map<string, Polygon>& Pols, istringstream& iss) {
    string name;
    iss >> name;
    string x_coord, y_coord;
    vp V;
    while (iss >> x_coord >> y_coord) {
        double x = stod(x_coord), y = stod(y_coord);
        Point P(x, y);
        V.push_back(P);
    }
    //Obs: new polygons are black.
    Pols[name] = Polygon(V);
    cout << "ok";
}


/* Prints the name and the vertices of a given polygon. */
void Polygon_print(map<string, Polygon>& Pols, istringstream& iss) {
    string name;
    if (iss >> name) {
        // Check of possible errors.
        if (undef_id(Pols, name)) return;
        if (wrong_number(iss)) return;
        cout << name;
        vp points = Pols[name].getPoints();
        int n = points.size();
        for (int i = 0; i < n; ++i) {
            cout << ' ' << points[i].getX() << ' ' << points[i].getY();
        }
    } else cout << "error: command with wrong number of arguments";
}


/* Prints the area of the given polygon. */
void Polygon_area(map<string, Polygon>& Pols, istringstream& iss) {
    string name;
    if (iss >> name) {
        if (undef_id(Pols, name)) return;
        if (wrong_number(iss)) return;
        cout << Pols[name].area();
    } else cout << "error: command with wrong number of arguments";
}


/* Prints the perimeter of the given polygon. */
void Polygon_perimeter(map<string, Polygon>& Pols, istringstream& iss) {
    string name;
    if (iss >> name) {
        if (undef_id(Pols, name)) return;
        if (wrong_number(iss)) return;
        cout << Pols[name].perimeter();
    } else cout << "error: command with wrong number of arguments";
}


/* Prints the number of vertices of the given polygon. */
void Polygon_vertices(map<string, Polygon>& Pols, istringstream& iss) {
    string name;
    if (iss >> name) {
        if (undef_id(Pols, name)) return;
        if (wrong_number(iss)) return;
        cout << Pols[name].vertices();
    } else cout << "error: command with wrong number of arguments";
}


/* Prints the centroid of the given polygon. */
void Polygon_centroid(map<string, Polygon>& Pols, istringstream& iss) {
    string name;
    if (iss >> name) {
        if (undef_id(Pols, name)) return;
        if (wrong_number(iss)) return;
        Point G = Pols[name].centroid();
        //double x = Pols[name].centroid().getX();
        //double y = Pols[name].centroid().getY();
        cout << G.getX() << ' ' << G.getY();
    } else cout << "error: command with wrong number of arguments";
}


/* Prints the number of edges of the given polygon. */
void Polygon_edges(map<string, Polygon>& Pols, istringstream& iss) {
    string name;
    if (iss >> name) {
        if (undef_id(Pols, name)) return;
        if (wrong_number(iss)) return;
        cout << Pols[name].edges();
    } else cout << "error: command with wrong number of arguments";
}


/* Prints yes or not to tell whether the given polygon is regular. */
void Polygon_regular(map<string, Polygon>& Pols, istringstream& iss) {
    string name;
    if (iss >> name) {
        if (undef_id(Pols, name)) return;
        if (wrong_number(iss)) return;
        if (Pols[name].regular()) cout << "yes";
        else cout << "not";
    } else cout << "error: command with wrong number of arguments";
}


/* Prints the width of the given polygon. */
void Polygon_width(map<string, Polygon>& Pols, istringstream& iss) {
    string name;
    if (iss >> name) {
        if (undef_id(Pols, name)) return;
        if (wrong_number(iss)) return;
        cout << Pols[name].width();
    } else cout << "error: command with wrong number of arguments";
}


/* Prints the height of the given polygon. */
void Polygon_height(map<string, Polygon>& Pols, istringstream& iss) {
    string name;
    if (iss >> name) {
        if (undef_id(Pols, name)) return;
        if (wrong_number(iss)) return;
        cout << Pols[name].height();
    } else cout << "error: command with wrong number of arguments";
}


/* Prints the RGB color of the given polygon. */
void Polygon_getcol(map<string, Polygon>& Pols, istringstream& iss) {
    string name;
    if (iss >> name) {
        if (undef_id(Pols, name)) return;
        if (wrong_number(iss)) return;
        Color c = Pols[name].getcol();
        cout << "R: " << c.R << " G: " << c.G << " B: " << c.B;
    } else cout << "error: command with wrong number of arguments";
}


/*  Lists all polygon identifiers, lexycographically sorted. */
void Polygon_list(map<string, Polygon>& Pols, istringstream& iss) {
    if (wrong_number(iss)) return;
    for (auto e : Pols) cout << e.first << ' ';
}


/* Saves a list of polygons in a file. */
void Polygon_save(map<string, Polygon>& Pols, istringstream& iss) {
    string file;
    if (iss >> file) {
        string name;
        vector <string> input;
        bool error = false;
        while (iss >> name and not error) {
            error = undef_id(Pols, name);
            input.push_back(name);
        }
        if (error) return;
        else {
            ofstream f(file);
            int m = input.size();
            for (int i = 0; i < m; ++i) {
                name = input[i]; 
                f << name;
                vp points = Pols[name].getPoints();
                int n = points.size();
                for (int i = 0; i < n; ++i) {
                    f << ' ' << points[i].getX() << ' ' << points[i].getY();
                }
                f << endl;
            }
            f.close();
            cout << "ok";
        }
    } else cout << "error: command with wrong number of arguments";
}


/* Loads the polygons stored in a file. */
void Polygon_load(map<string, Polygon>& Pols, istringstream& iss) {
    string file;
    if (iss >> file) {
        ifstream f(file);
        string line;
        while (getline(f, line)) {
            vp V;
            istringstream iss(line);
            string name, x_coord, y_coord;
            iss >> name;
            while (iss >> x_coord >> y_coord) {
                double x = stod(x_coord), y = stod(y_coord);
                Point P(x, y);
                V.push_back(P);
            }
            //Color c = {0, 0, 0};
            Pols.insert({name, Polygon(V)});
        }
        f.close();
        cout << "ok";
    } else cout << "error: command with wrong number of arguments";
}


/* Associates a color to the given polygon. */
void Polygon_setcol(map<string, Polygon>& Pols, istringstream& iss) {
    string name;
    if (iss >> name) {
        if (undef_id(Pols, name)) return;
        string r, g, b;
        if (iss >> r >> g >> b) {
            if (wrong_number(iss)) return;
            double R = stod(r), G = stod(g), B = stod(b);
            if (R > 1 or G > 1 or B > 1 or R < 0 or G < 0 or B < 0) {
                cout << "error: command with wrong type of arguments";
            } else {
                Color c = {R, G, B};
                Pols[name].setcol(c);
                cout << "ok";
            }
        } else {
            cout << "error: command with wrong number of arguments";  
        }
    } else cout << "error: command with wrong number of arguments";
}


/* Draws a list of polygons in a PNG file. */
void Polygon_draw(map<string, Polygon>& Pols, istringstream& iss) {
    const int size = 500;
    string image;
    if (iss >> image) {
        vector <string> input;
        string name;
        while (iss >> name) {
            if (undef_id(Pols, name)) return;
            input.push_back(name);
        }
        // create a png variable that denotes a size×size white canvas named image
        pngwriter png(size, size, 1.0, image.c_str());
        Polygon Box;
        for (int i = 0; i < (int)input.size(); ++i) {
            Box = Box.union_(Pols[input[i]]);
        }  
        Box = Box.bbox();
        vp pBox = Box.getPoints();
        double width = Box.width();
        double height = Box.height();
        double scale = (height > width ? height : width);
        scale = 498/scale;
        for (int i = 0; i < (int)input.size(); ++i) {
            vp points = Pols[input[i]].getPoints();
            Color c = Pols[input[i]].getcol();
            vector <int> scaled = {};
            int n = points.size(), x, y;
            for (int i = 0; i < n; ++i) {
                x = int((points[i].getX() - pBox[0].getX())*scale)+1;
                y = int((points[i].getY() - pBox[0].getY())*scale)+1;
                scaled.push_back(x);
                scaled.push_back(y);
            }
            x = int((points[0].getX() - pBox[0].getX())*scale)+1;
            y = int((points[0].getY() - pBox[0].getY())*scale)+1;
            scaled.push_back(x);
            scaled.push_back(y);
            png.polygon(scaled.data(), (int)scaled.size()/2, c.R, c.G, c.B);
        }
        cout << "ok";
        png.close();
    } else cout << "error: command with wrong number of arguments";
}


/* Stores the intersecion of two given polygons. */
void Polygon_intersection(map<string, Polygon>& Pols, istringstream& iss) {
    string p1, p2, p3;
    if (iss >> p1 >> p2) {
        if (undef_id(Pols, p2)) return;
        if (iss >> p3) {
            if (undef_id(Pols, p3)) return;
            if (wrong_number(iss)) return;
            Pols[p1] = Pols[p2].intersection(Pols[p3]);
        } else {
            if (undef_id(Pols, p1)) return;
            Pols[p1] = Pols[p1].intersection(Pols[p2]);
        }
        cout << "ok";
    } else cout << "error: command with wrong number of arguments";
}


/* Stores the union of two given polygons. */
void Polygon_union(map<string, Polygon>& Pols, istringstream& iss) {
    string p1, p2, p3;
    if (iss >> p1 >> p2) {
        if (undef_id(Pols, p2)) return;
        if (iss >> p3) {
            if (undef_id(Pols, p3)) return;
            if (wrong_number(iss)) return;
            Pols[p1] = Pols[p2].union_(Pols[p3]);
        } else {
            if (undef_id(Pols, p1)) return;
            Pols[p1] = Pols[p1].union_(Pols[p2]);
        }
        cout << "ok";
    } else cout << "error: command with wrong number of arguments";
}


/* Prints yes or not to tell whether the first polygon is inside the second. */
void Polygon_inside(map<string, Polygon>& Pols, istringstream& iss) {
    string name1, name2;
    if (iss >> name1 >> name2) {
        if (undef_id(Pols, name1)) return;
        if (undef_id(Pols, name2)) return;
        if (wrong_number(iss)) return;
        if (Pols[name1].inside(Pols[name2])) cout << "yes";
        else cout << "not";
    } else cout << "error: command with wrong number of arguments";
}


/* Computes the bounding box of the given polygons. */
void Polygon_bbox(map<string, Polygon>& Pols, istringstream& iss) {
    string bpol;
    if (iss >> bpol) {
        vp p;
        //Color c = {0,0,0};
        Pols[bpol] = Polygon(p);
        string name;
        while (iss >> name) Pols[bpol] = Pols[bpol].union_(Pols[name]);
        Pols[bpol] = Pols[bpol].bbox();
        cout << "ok";
    } else cout << "error: command with wrong number of arguments";
}


int main () {
    cout.setf(ios::fixed);
    cout.precision(3);
    map <string, Polygon> Pols;
    string s;
    while (getline(cin, s)) {
        istringstream iss(s);
        string action;
        iss >> action;
             if (action == "polygon")           Polygon_def(Pols, iss);
        else if (action == "print")             Polygon_print(Pols, iss);
        else if (action == "area")              Polygon_area(Pols, iss);
        else if (action == "perimeter")         Polygon_perimeter(Pols, iss);
        else if (action == "vertices")          Polygon_vertices(Pols, iss);
        else if (action == "centroid")          Polygon_centroid(Pols, iss);
        else if (action == "edges")             Polygon_edges(Pols, iss);
        else if (action == "regular")           Polygon_regular(Pols, iss);
        else if (action == "getcol")            Polygon_getcol(Pols, iss);
        else if (action == "width")             Polygon_width(Pols, iss);
        else if (action == "height")             Polygon_height(Pols, iss);
        else if (action == "list")              Polygon_list(Pols, iss);
        else if (action == "save")              Polygon_save(Pols, iss);
        else if (action == "load")              Polygon_load(Pols, iss);
        else if (action == "setcol")            Polygon_setcol(Pols, iss);
        else if (action == "draw")              Polygon_draw(Pols, iss);
        else if (action == "intersection")      Polygon_intersection(Pols, iss);
        else if (action == "union")             Polygon_union(Pols, iss);
        else if (action == "inside")            Polygon_inside(Pols, iss);
        else if (action == "bbox")              Polygon_bbox(Pols, iss);
        else if (action == "#") cout << "#";
        else cout << "error: invalid command";
        cout << endl;
    }
}

