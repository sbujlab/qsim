#ifndef __QSIMDETECTORCONSTRUCTION_HH
#define __QSIMDETECTORCONSTRUCTION_HH

#include "G4GDMLParser.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4Types.hh"

#include <vector>

class G4Tubs;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4VSensitiveDetector;
class G4GenericMessenger;

class qsimGlobalField;

class qsimDetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    qsimDetectorConstruction();
    virtual ~qsimDetectorConstruction();

  public:

    G4VPhysicalVolume* Construct();
    void ConstructSDandField();

    void SetDetectorGeomFile(const G4String& name) { fDetFileName = name; }

  private:

    G4GDMLParser *fGDMLParser;

    G4GenericMessenger* fMessenger;
    G4GenericMessenger* fGeometryMessenger;

    G4int fVerboseLevel;
    G4bool fGDMLValidate;
    G4bool fGDMLOverlapCheck;

    //----------------------
    // global magnet section
    //----------------------
    //

    static G4ThreadLocal qsimGlobalField* fGlobalField;

    G4String fDetFileName;

    G4VPhysicalVolume*      fWorldVolume;

  public:

    void DumpElements();
    void DumpMaterials();
    void DumpGeometry(G4bool surfchk = false) {
      DumpGeometricalTree(0,0,surfchk);
    }
    void DumpGeometricalTree(G4VPhysicalVolume* aVolume = 0,
      G4int depth = 0, G4bool surfchk = false);

  private:

    G4int UpdateCopyNo(G4VPhysicalVolume* aVolume, G4int index = 0);

};

#endif//__QSIMDETECTORCONSTRUCTION_HH
