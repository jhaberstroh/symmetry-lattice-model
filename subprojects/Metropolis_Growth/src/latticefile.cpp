#include "latticefile.h"
#include "interaction.h"
#include "lattice.h"

using namespace std;

/*--------------------------------------------------
  LatticeFile methods
  --------------------------------------------------*/

LatticeFile::LatticeFile(Lattice& lattice_to_save)
  :FileHandler(), m_lattice(lattice_to_save){
}

LatticeFile::RGBVect LatticeFile::ColorLookup(Color c, double* r, double* g, double* b){
  RGBVect rgb(3,0);
  switch (c){
  case kColorRed:
    //Firebrick red
    rgb[0] = 178.0/256; rgb[1] = 034.0/256; rgb[2] = 034.0/256;
    break;
  case kColorBlue:
    //Dark blue
    rgb[0] = 000.0/256; rgb[1] = 000.0/256; rgb[2] = 139.0/256;
    break;
  case kColorGreen:
    //Forest Green
    rgb[0] = 034.0/256; rgb[1] = 139.0/256; rgb[2] = 034.0/256;
    break;
  case kColorOrange:
    //Standard orange
    rgb[0] = 255.0/256; rgb[1] = 165.0/256; rgb[2] = 000.0/256;
    break;
  case kColorGrey:
    //Light grey
    rgb[0] = 211.0/256; rgb[1] = 211.0/256; rgb[2] = 211.0/256;
    break;
  case kColorBlack:
    //Very Dark grey
    rgb[0] = 054.0/256; rgb[1] = 054.0/256; rgb[2] = 054.0/256;
    break;
  default:
    //Default is ultra black
    rgb[0] = 000.0/256; rgb[1] = 000.0/256; rgb[2] = 000.0/256;
    break;
  }

  if (r != 0) (*r) = rgb[0];
  if (g != 0) (*g) = rgb[1];
  if (b != 0) (*b) = rgb[2];
  return rgb;
}


LatticeFile::Color LatticeFile::SelectSiteColor(int n1_bonds, int n2_bonds){
  //Must be used where N1 is the smaller symmetry number (more restrictive) than N2
  if (n2_bonds > n1_bonds && n1_bonds <= 1){
    //More nematic than ferro
    return kColorOrange;
  }
  if (n1_bonds >= 2 && n2_bonds >= 2){
    //only ferro bonds
    return kColorGreen;
  }
  if (n1_bonds >= 1 && n2_bonds >= 1){
    //light ferro bonds
    return kColorBlue;
  }
  if (n1_bonds <=0 && n2_bonds <= 0){
    //no bonding or anti-bonding
    return kColorGrey;
  }
  return kColorBlack;
}



void LatticeFile::DrawSite(pngwriter& png, int pixel_size, Lattice::Coord coord, int rot, int R, int n1_bonds, int n2_bonds){
  if (coord.size() == 2){
    if (rot != -1){
      Color site_color = SelectSiteColor(n1_bonds, n2_bonds);
      double r, g, b;
      ColorLookup(site_color, &r, &g, &b);

      int y = coord[0] * pixel_size;
      int x = coord[1] * pixel_size;
      //We are now in the upper-left corner.
      //Draw the square in the full pixel.
      png.filledsquare(x, y,
                 x+pixel_size,y+pixel_size,
                 r, g, b);

      int center_displacement = (pixel_size + 1) / 2;
      x += center_displacement;
      y += center_displacement;
      const double PI = 3.141592;
      double theta = ((double)rot / R) * 2 * PI;
      int x_head = - (center_displacement - 1) * sin(theta);
      int y_head =   (center_displacement - 1) * cos(theta);
      png.filledarrow(x - x_head, y - y_head,
                      x + x_head, y + y_head,
                      1, 20,
                      0,0,0);
    }
  }
}

void LatticeFile::MakeSquareLatticeColorImage(Interaction& lattice_interaction){
  vector<int> measurements = m_lattice.measurements();

  cout << "MAKING SQUARE LATTICE IMAGE?!" <<endl;
  //pixel_size should be odd
  int pixel_size = 25;
  pngwriter png(measurements[1]*pixel_size,measurements[0]*pixel_size,1.0,"testing.png");
  Lattice::Coord coord(2,0);
  Site* current_site;

  //draws a little mark in the corner for goodness, as suggested by Lester.
  png.square(1,1,2,2, 1,2,3);

  //draws all of the sites!
  for (int i = 0; i < measurements[0] ; i ++){
    for (int j = 0 ; j < measurements[1] ; j++){
      coord[0]=i; coord[1]=j;
      int n1_bonds = lattice_interaction.get_n1_bonds_at_site(coord);
      int n2_bonds = lattice_interaction.get_n2_bonds_at_site(coord);
      current_site = m_lattice.get_site(coord);

      int rot = (current_site->occ())? current_site->rot() : -1;
      DrawSite(png, pixel_size, coord, rot, m_lattice.R(), n1_bonds, n2_bonds);
    }
    cout << "iterating sites";
  }
  cout <<endl;
  png.close();
}


