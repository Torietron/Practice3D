xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 184;
 0.00000;297.12781;-3.70716;,
 0.00000;287.26742;-78.60406;,
 37.44846;287.26742;-68.56978;,
 64.86262;287.26742;-41.15562;,
 74.89690;287.26742;-3.70716;,
 64.86262;287.26742;33.74129;,
 37.44846;287.26742;61.15545;,
 0.00000;287.26742;71.18973;,
 -37.44840;287.26742;61.15545;,
 -64.86258;287.26742;33.74129;,
 -74.89690;287.26742;-3.70713;,
 -64.86262;287.26742;-41.15557;,
 -37.44849;287.26742;-68.56974;,
 0.00000;287.26742;-78.60406;,
 0.00000;258.35828;-148.39687;,
 72.34486;258.35828;-129.01214;,
 37.44846;287.26742;-68.56978;,
 125.30497;258.35828;-76.05202;,
 64.86262;287.26742;-41.15562;,
 144.68970;258.35828;-3.70716;,
 125.30497;258.35828;68.63769;,
 72.34486;258.35828;121.59781;,
 0.00004;258.35828;140.98255;,
 -72.34483;258.35828;121.59781;,
 -125.30492;258.35828;68.63773;,
 -144.68970;258.35828;-3.70707;,
 -125.30500;258.35828;-76.05194;,
 -72.34495;258.35828;-129.01209;,
 0.00000;258.35828;-148.39687;,
 0.00000;212.37042;-208.32929;,
 102.31108;212.37042;-180.91511;,
 177.20796;212.37042;-106.01824;,
 204.62212;212.37042;-3.70716;,
 177.20793;212.37042;98.60391;,
 102.31108;212.37042;173.50078;,
 0.00004;212.37042;200.91496;,
 -102.31104;212.37042;173.50082;,
 -177.20793;212.37042;98.60397;,
 -204.62212;212.37042;-3.70707;,
 -177.20802;212.37042;-106.01813;,
 -102.31121;212.37042;-180.91507;,
 0.00000;212.37042;-208.32929;,
 0.00000;152.43805;-254.31710;,
 125.30497;152.43805;-220.74171;,
 217.03457;152.43805;-129.01214;,
 250.60994;152.43805;-3.70716;,
 217.03457;152.43805;121.59781;,
 125.30497;152.43805;213.32742;,
 0.00004;152.43805;246.90277;,
 -125.30492;152.43805;213.32746;,
 -217.03451;152.43805;121.59787;,
 -250.60994;152.43805;-3.70704;,
 -217.03464;152.43805;-129.01200;,
 -125.30512;152.43805;-220.74164;,
 0.00000;152.43805;-254.31710;,
 0.00000;82.64523;-283.22628;,
 139.75955;82.64523;-245.77777;,
 242.07064;82.64523;-143.46668;,
 279.51910;82.64523;-3.70716;,
 242.07062;82.64523;136.05239;,
 139.75955;82.64523;238.36343;,
 0.00004;82.64523;275.81195;,
 -139.75948;82.64523;238.36347;,
 -242.07062;82.64523;136.05246;,
 -279.51910;82.64523;-3.70704;,
 -242.07066;82.64523;-143.46656;,
 -139.75972;82.64523;-245.77769;,
 0.00000;82.64523;-283.22628;,
 0.00002;0.00002;-293.08660;,
 144.68970;0.00002;-254.31710;,
 250.60994;0.00000;-148.39687;,
 289.37943;0.00000;-3.70716;,
 250.60994;0.00000;140.98255;,
 144.68970;-0.00002;246.90277;,
 0.00002;-0.00002;285.67226;,
 -144.68975;-0.00002;246.90273;,
 -250.60994;0.00000;140.98250;,
 -289.37943;0.00000;-3.70716;,
 -250.60994;0.00000;-148.39683;,
 -144.68975;0.00002;-254.31706;,
 0.00002;0.00002;-293.08660;,
 0.00000;-297.12781;-3.70718;,
 0.00000;-287.26742;71.18972;,
 37.44846;-287.26742;61.15544;,
 64.86262;-287.26742;33.74128;,
 74.89690;-287.26742;-3.70718;,
 64.86262;-287.26742;-41.15564;,
 37.44846;-287.26742;-68.56980;,
 0.00000;-287.26742;-78.60410;,
 -37.44840;-287.26742;-68.56980;,
 -64.86258;-287.26742;-41.15564;,
 -74.89690;-287.26742;-3.70722;,
 -64.86262;-287.26742;33.74122;,
 -37.44849;-287.26742;61.15540;,
 0.00000;-258.35833;140.98255;,
 72.34486;-258.35828;121.59781;,
 125.30497;-258.35828;68.63766;,
 144.68970;-258.35828;-3.70718;,
 125.30497;-258.35828;-76.05204;,
 72.34486;-258.35828;-129.01217;,
 0.00004;-258.35824;-148.39690;,
 -72.34483;-258.35828;-129.01217;,
 -37.44840;-287.26742;-68.56980;,
 -72.34483;-258.35828;-129.01217;,
 -125.30492;-258.35828;-76.05208;,
 -64.86258;-287.26742;-41.15564;,
 -144.68970;-258.35828;-3.70727;,
 -74.89690;-287.26742;-3.70722;,
 -125.30500;-258.35828;68.63759;,
 -64.86262;-287.26742;33.74122;,
 -72.34495;-258.35828;121.59772;,
 -37.44849;-287.26742;61.15540;,
 0.00000;-212.37044;200.91496;,
 102.31108;-212.37044;173.50074;,
 177.20796;-212.37042;98.60389;,
 204.62212;-212.37042;-3.70718;,
 177.20793;-212.37042;-106.01826;,
 102.31108;-212.37039;-180.91511;,
 0.00004;-212.37039;-208.32929;,
 -102.31104;-212.37039;-180.91515;,
 -102.31104;-212.37039;-180.91515;,
 -177.20793;-212.37042;-106.01832;,
 -204.62212;-212.37042;-3.70727;,
 -177.20802;-212.37042;98.60377;,
 -102.31121;-212.37044;173.50074;,
 0.00000;-152.43805;246.90277;,
 125.30497;-152.43805;213.32738;,
 217.03457;-152.43805;121.59781;,
 250.60994;-152.43805;-3.70718;,
 217.03457;-152.43803;-129.01214;,
 125.30497;-152.43803;-220.74175;,
 0.00004;-152.43803;-254.31714;,
 -125.30492;-152.43803;-220.74179;,
 -125.30492;-152.43803;-220.74179;,
 -217.03451;-152.43803;-129.01219;,
 -250.60994;-152.43805;-3.70731;,
 -217.03464;-152.43805;121.59765;,
 -125.30512;-152.43805;213.32730;,
 0.00000;-82.64524;275.81195;,
 139.75955;-82.64524;238.36343;,
 242.07064;-82.64524;136.05233;,
 279.51910;-82.64523;-3.70716;,
 242.07062;-82.64521;-143.46674;,
 139.75955;-82.64521;-245.77777;,
 0.00004;-82.64521;-283.22628;,
 -139.75948;-82.64521;-245.77781;,
 -139.75948;-82.64521;-245.77781;,
 -242.07062;-82.64521;-143.46678;,
 -279.51910;-82.64523;-3.70729;,
 -242.07066;-82.64524;136.05223;,
 -139.75972;-82.64524;238.36339;,
 0.00002;0.05665;288.57331;,
 146.14023;0.05665;249.41511;,
 253.12228;0.05666;142.43306;,
 292.28046;0.05666;-3.70716;,
 253.12228;0.05666;-149.84738;,
 146.14023;0.05668;-256.82945;,
 0.00002;0.05668;-295.98761;,
 -146.14029;0.05668;-256.82937;,
 -146.14029;0.05668;-256.82937;,
 -253.12226;0.05666;-149.84736;,
 -292.28046;0.05666;-3.70716;,
 -253.12226;0.05666;142.43303;,
 -146.14029;0.05665;249.41503;,
 255.75464;-31.49833;-131.18177;,
 250.46732;40.46562;-122.07139;,
 132.22180;43.86690;-229.29312;,
 137.50922;-28.09706;-238.40351;,
 137.50922;-28.09706;-238.40351;,
 132.22180;43.86690;-229.29312;,
 -18.72450;40.41257;-280.16038;,
 -13.43729;-31.55141;-289.27062;,
 -240.58050;41.92081;-135.89156;,
 -263.51128;40.74912;-54.79411;,
 -268.27224;-31.21488;-64.19008;,
 -245.34140;-30.04315;-145.28763;,
 -142.75488;-27.76052;-246.81148;,
 -137.99407;44.20345;-237.41519;,
 -5.23735;41.56277;-270.28847;,
 -137.99407;44.20345;-237.41519;,
 -142.75488;-27.76052;-246.81148;,
 -9.99824;-30.40119;-279.68460;,
 36.67984;-31.17641;-264.98324;,
 41.44075;40.78753;-255.58718;;
 
 150;
 3;0,1,2;,
 3;0,2,3;,
 3;0,3,4;,
 3;0,4,5;,
 3;0,5,6;,
 3;0,6,7;,
 3;0,7,8;,
 3;0,8,9;,
 3;0,9,10;,
 3;0,10,11;,
 3;0,11,12;,
 3;0,12,1;,
 4;13,14,15,16;,
 4;16,15,17,18;,
 4;18,17,19,4;,
 4;4,19,20,5;,
 4;5,20,21,6;,
 4;6,21,22,7;,
 4;7,22,23,8;,
 4;8,23,24,9;,
 4;9,24,25,10;,
 4;10,25,26,11;,
 4;11,26,27,12;,
 4;12,27,28,1;,
 4;14,29,30,15;,
 4;15,30,31,17;,
 4;17,31,32,19;,
 4;19,32,33,20;,
 4;20,33,34,21;,
 4;21,34,35,22;,
 4;22,35,36,23;,
 4;23,36,37,24;,
 4;24,37,38,25;,
 4;25,38,39,26;,
 4;26,39,40,27;,
 4;27,40,41,28;,
 4;29,42,43,30;,
 4;30,43,44,31;,
 4;31,44,45,32;,
 4;32,45,46,33;,
 4;33,46,47,34;,
 4;34,47,48,35;,
 4;35,48,49,36;,
 4;36,49,50,37;,
 4;37,50,51,38;,
 4;38,51,52,39;,
 4;39,52,53,40;,
 4;40,53,54,41;,
 4;42,55,56,43;,
 4;43,56,57,44;,
 4;44,57,58,45;,
 4;45,58,59,46;,
 4;46,59,60,47;,
 4;47,60,61,48;,
 4;48,61,62,49;,
 4;49,62,63,50;,
 4;50,63,64,51;,
 4;51,64,65,52;,
 4;52,65,66,53;,
 4;53,66,67,54;,
 4;55,68,69,56;,
 4;56,69,70,57;,
 4;57,70,71,58;,
 4;58,71,72,59;,
 4;59,72,73,60;,
 4;60,73,74,61;,
 4;61,74,75,62;,
 4;62,75,76,63;,
 4;63,76,77,64;,
 4;64,77,78,65;,
 4;65,78,79,66;,
 4;66,79,80,67;,
 3;81,82,83;,
 3;81,83,84;,
 3;81,84,85;,
 3;81,85,86;,
 3;81,86,87;,
 3;81,87,88;,
 3;81,88,89;,
 3;81,89,90;,
 3;81,90,91;,
 3;81,91,92;,
 3;81,92,93;,
 3;81,93,82;,
 4;82,94,95,83;,
 4;83,95,96,84;,
 4;84,96,97,85;,
 4;85,97,98,86;,
 4;86,98,99,87;,
 4;87,99,100,88;,
 4;88,100,101,89;,
 4;102,103,104,105;,
 4;105,104,106,107;,
 4;107,106,108,109;,
 4;109,108,110,111;,
 4;111,110,94,82;,
 4;94,112,113,95;,
 4;95,113,114,96;,
 4;96,114,115,97;,
 4;97,115,116,98;,
 4;98,116,117,99;,
 4;99,117,118,100;,
 4;100,118,119,101;,
 4;103,120,121,104;,
 4;104,121,122,106;,
 4;106,122,123,108;,
 4;108,123,124,110;,
 4;110,124,112,94;,
 4;112,125,126,113;,
 4;113,126,127,114;,
 4;114,127,128,115;,
 4;115,128,129,116;,
 4;116,129,130,117;,
 4;117,130,131,118;,
 4;118,131,132,119;,
 4;120,133,134,121;,
 4;121,134,135,122;,
 4;122,135,136,123;,
 4;123,136,137,124;,
 4;124,137,125,112;,
 4;125,138,139,126;,
 4;126,139,140,127;,
 4;127,140,141,128;,
 4;128,141,142,129;,
 4;129,142,143,130;,
 4;130,143,144,131;,
 4;131,144,145,132;,
 4;133,146,147,134;,
 4;134,147,148,135;,
 4;135,148,149,136;,
 4;136,149,150,137;,
 4;137,150,138,125;,
 4;138,151,152,139;,
 4;139,152,153,140;,
 4;140,153,154,141;,
 4;141,154,155,142;,
 4;142,155,156,143;,
 4;143,156,157,144;,
 4;144,157,158,145;,
 4;146,159,160,147;,
 4;147,160,161,148;,
 4;148,161,162,149;,
 4;149,162,163,150;,
 4;150,163,151,138;,
 4;164,165,166,167;,
 4;168,169,170,171;,
 4;172,173,174,175;,
 4;172,175,176,177;,
 4;178,179,180,181;,
 4;178,181,182,183;;
 
 MeshMaterialList {
  48;
  150;
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  3,
  3,
  3,
  3,
  3,
  3;;
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.031000;0.212000;1.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_skydome2_2.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_skydome2_1.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_distant1.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_sea1.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_forest2.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_forest1.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_mt3.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;0.990000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_mt2.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;0.990000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_mt1_2.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_beach1.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_beach2.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_grass1.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_grass2.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_cliff1.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.750000;0.750000;0.750000;;
   TextureFilename {
    "tx\\tx_cliff1.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_cliff2.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_cliff3.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_cliff4.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_cliff6.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_rock5.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.700000;0.700000;0.700000;;
   TextureFilename {
    "tx\\tx_rock5.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.500000;0.500000;0.500000;;
   TextureFilename {
    "tx\\tx_rock5.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_rock2.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.600000;0.600000;0.600000;;
   TextureFilename {
    "tx\\tx_rock2.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_cave1.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_frock1.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_frock2.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_river1.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_wood1.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_falls1.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_gtree3.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_foliage1.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_stree3.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_gtree4.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_gtree3.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;0.200000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_wave1.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;0.700000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_wave2.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_wave3.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_mgrass1.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_stree2.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_stree3.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_stree1.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_flowerb1.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_flower1.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_palm1.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_foliage1.tga";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "tx\\tx_sprig1.tga";
   }
  }
 }
 MeshTextureCoords {
  184;
  0.749560;-0.000810;,
  0.999960;0.080390;,
  1.083150;0.080390;,
  1.166350;0.080390;,
  0.251240;0.080390;,
  0.334430;0.080390;,
  0.417620;0.080390;,
  0.500810;0.080390;,
  0.584000;0.080390;,
  0.667200;0.080390;,
  0.750390;0.080390;,
  0.833580;0.080390;,
  0.916780;0.080390;,
  0.001660;0.080390;,
  0.001660;0.258910;,
  0.084850;0.258910;,
  0.084850;0.080390;,
  0.168050;0.258910;,
  0.168050;0.080390;,
  0.251240;0.258910;,
  0.334430;0.258910;,
  0.417620;0.258910;,
  0.500810;0.258910;,
  0.584000;0.258910;,
  0.667200;0.258910;,
  0.750390;0.258910;,
  0.833580;0.258910;,
  0.916780;0.258910;,
  0.999960;0.258910;,
  0.001660;0.438010;,
  0.084850;0.438010;,
  0.168050;0.438010;,
  0.251240;0.438010;,
  0.334430;0.438010;,
  0.417620;0.438010;,
  0.500810;0.438010;,
  0.584000;0.438010;,
  0.667200;0.438010;,
  0.750390;0.438010;,
  0.833580;0.438010;,
  0.916780;0.438010;,
  0.999960;0.438010;,
  0.001660;0.617930;,
  0.084850;0.617930;,
  0.168050;0.617930;,
  0.251240;0.617930;,
  0.334430;0.617930;,
  0.417620;0.617930;,
  0.500810;0.617930;,
  0.584000;0.617930;,
  0.667200;0.617930;,
  0.750390;0.617930;,
  0.833580;0.617930;,
  0.916780;0.617930;,
  0.999960;0.617930;,
  0.001660;0.798900;,
  0.084850;0.798900;,
  0.168050;0.798900;,
  0.251240;0.798900;,
  0.334430;0.798900;,
  0.417620;0.798900;,
  0.500810;0.798900;,
  0.584000;0.798900;,
  0.667200;0.798900;,
  0.750390;0.798900;,
  0.833580;0.798900;,
  0.916780;0.798900;,
  0.999960;0.798900;,
  0.001700;0.999000;,
  0.084880;0.999000;,
  0.168080;0.999000;,
  0.251270;0.999000;,
  0.334460;0.999000;,
  0.417660;0.999000;,
  0.500850;0.999000;,
  0.584040;0.999000;,
  0.667240;0.999000;,
  0.750420;0.999000;,
  0.833610;0.999000;,
  0.916810;0.999000;,
  1.000000;0.999000;,
  -9.140440;5.532000;,
  -3.611520;4.634320;,
  -4.532970;4.634320;,
  -5.454530;4.634320;,
  -6.375980;4.634320;,
  -7.297430;4.634320;,
  -8.218990;4.634320;,
  -9.140440;4.634320;,
  -10.061890;4.634320;,
  -10.983450;4.634320;,
  -11.904910;4.634320;,
  -12.826360;4.634320;,
  -13.747920;4.634320;,
  -3.611520;3.735100;,
  -4.532970;3.735100;,
  -5.454530;3.735100;,
  -6.375980;3.735100;,
  -7.297430;3.735100;,
  -8.218990;3.735100;,
  -9.140440;3.735100;,
  -10.061890;3.735100;,
  0.995950;4.634320;,
  0.995950;3.735100;,
  0.074390;3.735100;,
  0.074390;4.634320;,
  -0.847060;3.735100;,
  -0.847060;4.634320;,
  -1.768510;3.735100;,
  -1.768510;4.634320;,
  -2.690070;3.735100;,
  -2.690070;4.634320;,
  -3.611520;2.832890;,
  -4.532970;2.832890;,
  -5.454530;2.832890;,
  -6.375980;2.832890;,
  -7.297430;2.832890;,
  -8.218990;2.832890;,
  -9.140440;2.832890;,
  -10.061890;2.832890;,
  0.995950;2.832890;,
  0.074390;2.832890;,
  -0.847060;2.832890;,
  -1.768510;2.832890;,
  -2.690070;2.832890;,
  -3.611520;1.926590;,
  -4.532970;1.926590;,
  -5.454530;1.926590;,
  -6.375980;1.926590;,
  -7.297430;1.926590;,
  -8.218990;1.926590;,
  -9.140440;1.926590;,
  -10.061890;1.926590;,
  0.995950;1.926590;,
  0.074390;1.926590;,
  -0.847060;1.926590;,
  -1.768510;1.926590;,
  -2.690070;1.926590;,
  -3.611520;1.014980;,
  -4.532970;1.014980;,
  -5.454530;1.014980;,
  -6.375980;1.014980;,
  -7.297430;1.014980;,
  -8.218990;1.014980;,
  -9.140440;1.014980;,
  -10.061890;1.014980;,
  0.995950;1.014980;,
  0.074390;1.014980;,
  -0.847060;1.014980;,
  -1.768510;1.014980;,
  -2.690070;1.014980;,
  -3.611520;0.003080;,
  -4.532970;0.003080;,
  -5.454530;0.003080;,
  -6.375980;0.003080;,
  -7.297430;0.003080;,
  -8.218990;0.003080;,
  -9.140440;0.003080;,
  -10.061890;0.003080;,
  0.995950;0.003080;,
  0.074390;0.003080;,
  -0.847060;0.003080;,
  -1.768510;0.003080;,
  -2.690070;0.003080;,
  0.000000;0.500000;,
  0.000000;0.010000;,
  0.966330;0.010000;,
  0.966330;0.500000;,
  0.032330;0.998650;,
  0.032330;0.508650;,
  1.000000;0.510000;,
  1.000000;1.000000;,
  0.741810;0.506850;,
  0.994610;0.507310;,
  0.993260;0.995960;,
  0.741810;0.996850;,
  0.032330;0.998650;,
  0.032330;0.508650;,
  0.197110;0.010000;,
  0.966330;0.010000;,
  0.966330;0.500000;,
  0.197110;0.500000;,
  0.000000;0.500000;,
  0.000000;0.010000;;
 }
}
