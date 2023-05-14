/***********************************************************************
 * The rDock program was developed from 1998 - 2006 by the software team
 * at RiboTargets (subsequently Vernalis (R&D) Ltd).
 * In 2006, the software was licensed to the University of York for
 * maintenance and distribution.
 * In 2012, Vernalis and the University of York agreed to release the
 * program as Open Source software.
 * This version is licensed under GNU-LGPL version 3.0 with support from
 * the University of Barcelona.
 * http://rdock.sourceforge.net/
 ***********************************************************************/

// The role of RbtDockingSite is to manage the
// list of cavities generated by the site mapper, and to store a 3-D grid of
// precalculated distances to the nearest cavity coordinate. This grid can be
// used by scoring function classes to lookup the list of atoms required to
// calculate the score component.

#ifndef _RBTDOCKINGSITE_H_
#define _RBTDOCKINGSITE_H_

#include "RbtAtom.h"
#include "RbtCavity.h"
#include "RbtConfig.h"
#include "RbtError.h"
#include "RbtRealGrid.h"

class RbtDockingSite {
 public:
    // STL predicate for selecting atoms within a defined distance range from nearest cavity coords
    // Uses precalculated distance grid
    class isAtomInRange: public std::unary_function<RbtAtomPtr, RbtBool> {
     public:
        explicit isAtomInRange(RbtRealGrid* pGrid, RbtDouble minDist, RbtDouble maxDist):
            m_pGrid(pGrid),
            m_minDist(minDist),
            m_maxDist(maxDist) {}
        RbtBool operator()(RbtAtom* pAtom) const;

     private:
        RbtRealGrid* m_pGrid;
        RbtDouble m_minDist;
        RbtDouble m_maxDist;
    };

 public:
    // Class type string
    static RbtString _CT;

    RbtDockingSite(const RbtCavityList& cavList, RbtDouble border);
    RbtDockingSite(istream& istr);

    // Destructor
    ~RbtDockingSite();

    // Insertion operator
    friend ostream& operator<<(ostream& s, const RbtDockingSite& site);

    // Virtual function for dumping docking site details to an output stream
    // Derived classes can override if required
    virtual void Print(ostream& s) const;

    // Public methods
    void Read(istream& istr);   // Reads docking site from binary stream
    void Write(ostream& ostr);  // Writes docking site to binary stream

    RbtRealGridPtr GetGrid();
    RbtDouble GetBorder() const { return m_border; }
    RbtCoord GetMinCoord() const { return m_minCoord; }
    RbtCoord GetMaxCoord() const { return m_maxCoord; }
    RbtCavityList GetCavityList() const { return m_cavityList; }
    RbtInt GetNumCavities() const { return m_cavityList.size(); }
    RbtDouble GetVolume() const;  // returns total volume of all cavities in A^3

    // Returns the combined coord lists of all the cavities
    void GetCoordList(RbtCoordList& retVal) const;

    // Filters an atom list according to distance from the cavity coords
    // Only returns atoms within minDist and maxDist from cavity
    RbtAtomList GetAtomList(const RbtAtomList& atomList, RbtDouble minDist, RbtDouble maxDist) throw(RbtError);
    // Filters an atom list according to distance from the cavity coords
    // Only returns atoms within maxDist from cavity
    // This version does not require the cavity grid
    RbtAtomList GetAtomList(const RbtAtomList& atomList, RbtDouble maxDist) throw(RbtError);
    // Returns the count of atoms within minDist and maxDist from cavity
    RbtUInt GetNumAtoms(const RbtAtomList& atomList, RbtDouble minDist, RbtDouble maxDist) throw(RbtError);

    // private methods
 private:
    RbtDockingSite();                                  // default constructor disabled
    RbtDockingSite(const RbtDockingSite&);             // Copy constructor disabled by default
    RbtDockingSite& operator=(const RbtDockingSite&);  // Copy assignment disabled by default

    void CreateGrid();

    // Private data
 private:
    RbtCavityList m_cavityList;  // Cavity list
    RbtCoord m_minCoord;
    RbtCoord m_maxCoord;
    RbtRealGridPtr m_spGrid;
    RbtDouble m_border;
};

// Useful typedefs
typedef SmartPtr<RbtDockingSite> RbtDockingSitePtr;  // Smart pointer

#endif  //_RBTDOCKINGSITE_H_
