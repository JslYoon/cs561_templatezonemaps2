#ifndef ZONEMAPS_H
#define ZONEMAPS_H
#include <iostream>
#include <vector>
#include <unordered_map>

template <typename T>
struct zone
{
    std::vector<T> elements; // vector/list that holds elements in this zone
    T min;                   // minimum element in the zone
    T max;                   // maximum element in the zone
    uint size;               // no. of elements in current zone
    bool sorted;          // whether this zone is sorted or not
};

template <typename T>
class zonemap
{
    std::vector<T> elements; // vector/list that will hold all elements. Can convert to array for faster processing
    std::vector<zone<T> > zones;
    uint num_zones;             // keeps track of the number of zones/partitions to the array/vector of elements
    uint num_elements_per_zone; // keeps track of number of elements per zone (items in every zone has to be less than or equal to this counter)

public:
    /**
     * Purpose: basic constructor that initializes all data members. 
     * Param: number of elements per zone 
     * returns: N/A 
     */
    zonemap(std::vector<T> _elements, uint _num_elements_per_zone);

    /**
     * Purpose: builds the zonemap using the elements array/list. Maintains invariant that no. of elements in every zone
     *          is less than or equal to num_elements_per_zone 
     * Param: N/A 
     * returns: N/A 
     */
    void build();

    /**
     * Purpose: sorts the elements in the vector/list/array. 
     * Param: N/A 
     * returns: N/A 
     */
    void sort_elements();

    /**
     * Purpose: Query a key using the zonemap 
     * Param: Key to be queried  
     * returns: the number of found keys 
     */
    size_t query(T key);

    /**
     * Purpose: Query a range of keys using the zonemap that are between a lower and higher limit
     * Param: Lower and upper limit of range to be queried  
     * returns: list of elements found using the query 
     */
    std::vector<T> query(T low, T high);

    // Purpose: sort the elements in a zone
    // Param: index of the zone to be sorted
    // returns: N/A
    void sort_zone(int index);

    // Purpose: split a zone into two smaller zones in the zones vector
    // Param: index of the zone to split
    // returns: N/A
    void split_zone(int index);

    // Purpose: find the zone where the key will belong to
    // Param: key to find the zone for
    // Returns: index of the zone in zones vector
    int find_zone(T key);

};
#endif
