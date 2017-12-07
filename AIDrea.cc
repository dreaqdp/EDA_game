#include "Player.hh"
#include <map>
#include<queue>
#include<list>


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Drea


struct PLAYER_NAME : public Player {

    /**
    * Factory: returns a new instance of this class.
    * Do not modify this function.
    */
    static Player* factory () {
    return new PLAYER_NAME;
    }
    
    map <int, City> cities;
    map <int, Path> paths;
    
    typedef vector <int> node;
    typedef vector <node> graph;

    void move (int id_ork) {
        Unit ork = unit (id_ork);
        Dir next = TOP;
        if (dir_ok(next)) {
            ork.pos += next;
            if (pos_ok(ork.pos)) execute(Command(id_ork, next));
        }
        return;
        
    }
    
    //Find cities and paths of the board
    void find_c_p () {
        Cell c;
        //map<int,city>::iterator itc = itc.begin();
        //map<int,path>::iterator itp = itp.begin();
        
        for (int i = 0; i < rows(); ++i) {
            for (int j = 0; j < cols(); ++j) {
                c = cell(i,j);
                if (c.type == CITY) {
                    if (cities.find(c.city_id) == cities.end()) 
                        cities.insert({c.city_id, city(c.city_id)});
                    
                }
                else if (c.type == PATH) {
                    if (paths.find(c.path_id) == paths.end()) 
                        paths.insert({c.path_id, path(c.path_id)});
                    
                }
            }
        }
    }
    
    void bfs (const Pos upos, list<Dir> & way) {
        vector <bool> visited (rows()*cols() ,false); //aplicar acces aleatori de EC a les matrius
        queue <Pos> q;
        q.push(upos);
        bool found = false;
        
        
        while (not q.empty() and not found) {
            Pos p = q.front();
            q.pop();
            int posp = p.i*cols()+p.j;
            if (not visited[posp]) {
                visited[posp] = true;
                for (int i = 0; i < 4; ++i) {
                    Dir d;
                    if (i == 0) d = BOTTOM;
                    else if (i == 1) d = RIGHT;
                    else if (i == 2) d = TOP;
                    else d = LEFT;
                    
                    Cell c = cell(p+d);
                    if (not c.type == WATER) q.push(p+d);
                    if (c.type == CITY or c.type == PATH) {
                        way.push_back(d);
                        found = true;
                        
                    }
                    else way.push_back(d);
                    
                }
                
            }
        }
    }
    

    /**
    * Types and attributes for your player can be defined here.
    */

    /**
    * Play method, invoked once per each round.
    */
    virtual void play () {
        vector <int> my_orks = orks(me());
        for (int i = 0; i < int(my_orks.size()); ++i) {
            move (my_orks[i]);
        }
        
        //buscar ciutats
        if (round() == 0) {
            find_c_p();
            
            /*map<int, City>::iterator itc;
            itc = cities.begin();
            map<int, Path>::iterator itp = paths.begin();
            for (itc = cities.begin(); itc != cities.end(); ++itc) {
                cerr << "City with id: " << (*itc).first << " at " << itc->second[1] << endl;
            }
            for (itp = paths.begin(); itp != paths.end(); ++itp) {
                cerr << "Path with id: " << (*itp).first;
                cerr << " connects " << (*itp).second.first.first << " and " << (*itp).second.first.second << endl;
             }*/
            Unit o = unit(my_orks[0]);
            list<Dir> way;
            bfs(o.pos, way);
            cerr << "cami a seguir per l'ork " << o.id << " es:";
            for(list<Dir>::iterator it = way.begin(); it != way.end(); ++it) {
                cerr << *it << ' ';
            }
            cerr << endl;
        }
        
        
        
    }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
