#include "zonemaps.h"


template<typename T>
zonemap<T>::zonemap(std::vector<T> _elements, uint _num_elements_per_zone) {
    // constructor 
    // Your code starts here ...
    num_elements_per_zone = _num_elements_per_zone;
    num_zones = 1;
    zone<T> z1;
    zones.push_back(z1);
    elements = _elements;
    build();
}

template<typename T>
void zonemap<T>::build() {
    int count = 1;
    for (T i : elements) {
        if (num_zones == 1 && zones[0].elements.size() == 0) {
            zones[0].elements.push_back(i);
            zones[0].max = i;
            zones[0].min = i;
            num_zones++;
            continue;
        }

        int ind = find_zone(i);
        zone<T>& z = zones[ind];

        if (z.elements.size() < num_elements_per_zone) {
            z.sorted = false;
            z.elements.push_back(i);
            if (i < z.min) {
                z.min = i;
            } else if (i > z.max) {
                z.max = i;
            }
            z.size++;
        } else {
            z.elements.push_back(i);
            split_zone(ind);
        }
        count++;
    }
}


template<typename T>
int zonemap<T>::find_zone(T key) {
   for (size_t i = 0; i < zones.size(); ++i) {
        if (zones[i].max > key && zones[i].min < key) {
            return static_cast<int>(i);
        }
    }
    if (zones.back().max < key) {
        return static_cast<int>(zones.size() - 1); 
    } else {
        return 0;
    }
}


template<typename T>
void zonemap<T>::split_zone(int index) {
    sort_zone(index);
    zone<T>& z = zones[index];
    size_t splitPoint = z.elements.size() / 2;
    for ( ; splitPoint < z.elements.size()-2 ; splitPoint++) {
        if(z.elements[splitPoint] != z.elements[splitPoint+1]) {
            break;
        }

    }
    zone<T> z1;
    zone<T> z2;


    z1.elements.assign(z.elements.begin(), z.elements.begin() + splitPoint);
    z2.elements.assign(z.elements.begin() + splitPoint, z.elements.end());


    z1.min = z1.elements[0];
    z1.max = z1.elements.back();
    z1.sorted = true;

    z2.min = z2.elements[0];
    z2.max = z2.elements.back();
    z2.sorted = true;

    zones.erase(zones.begin() + index);
    zones.insert(zones.begin() + index, z2);
    zones.insert(zones.begin() + index, z1); 
    num_zones++;
}


template<typename T>
void zonemap<T>::sort_zone(int index) {

    zone<T>& z = zones[index];
    std::sort(z.elements.begin(), z.elements.end());
    z.sorted = true;

}


template<typename T>
size_t zonemap<T>::query(T key) {

    int index = find_zone(key);

    size_t count = 0;
    for (const auto &element : zones[index].elements) {
        if (element == key) {
            ++count;
        }
    }
    if (index + 1 < zones.size() && zones[index + 1].min == key) {
        for (const auto &element : zones[index + 1].elements) {
            if (element == key) {
                ++count;
            }
        }
    }
    return count;
}

template<typename T>
std::vector<T> zonemap<T>::query(T low, T high) {
    std::vector<T> vec;
    int l = find_zone(low);
    int h = find_zone(high);
    for (int i = l + 1; i < h; i++) {
        if (i >= 0 && i < zones.size()) {
            vec.insert(vec.end(), zones[i].elements.begin(), zones[i].elements.end());
        }
    }
    for (int i = 0; i < zones[l].elements.size(); i++) {
        vec.push_back(zones[l].elements[i]);
    }
    if (l != h) {
        for (int i = 0; i < zones[h].elements.size(); i++) {
            vec.push_back(zones[h].elements[i]);
        }
    }
    return vec;
}


template<typename T>
void zonemap<T>::sort_elements() {
    std::vector<T> temp;
    for (size_t i = 0; i < zones.size(); ++i) {
        if (zones[i].sorted == true) {
            temp.insert(temp.end(), zones[i].elements.begin(), zones[i].elements.end());
        } else {
            sort_zone(i);
            temp.insert(temp.end(), zones[i].elements.begin(), zones[i].elements.end());
        }
    } 
    elements = temp;

}