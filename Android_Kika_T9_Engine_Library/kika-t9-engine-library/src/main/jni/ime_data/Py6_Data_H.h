#ifndef __Py6_DATA_UI_H_
#define __Py6_DATA_UI_H_

#define MAX_Py6_DATA_UI                52

struct Py6_DATA_UI
{
       struct Py6_DATA_ID
       {
            wchar_t*        WData;
            wchar_t*        OData;
       };
       
       Py6_DATA_ID
       DataUI[MAX_Py6_DATA_UI];
       
       ~Py6_DATA_UI()
       {
            for ( int i =0; i <MAX_Py6_DATA_UI; i++ )
            {
                  if ( DataUI[i].WData ) 
                       delete DataUI[i].WData;
            }
       }

       Py6_DATA_UI()
       {
            //------------------------------------------------------
            // Index 0
            //------------------------------------------------------
            DataUI[ 0].WData =new wchar_t[6 +1];
            DataUI[ 0].WData[ 0] =0x540c;
            DataUI[ 0].WData[ 1] =0x3a;
            DataUI[ 0].WData[ 2] =0x74;
            DataUI[ 0].WData[ 3] =0x6f;
            DataUI[ 0].WData[ 4] =0x6e;
            DataUI[ 0].WData[ 5] =0x67;
            DataUI[ 0].WData[ 6] =0;

            //------------------------------------------------------
            // Index 1
            //------------------------------------------------------
            DataUI[ 1].WData =new wchar_t[6 +1];
            DataUI[ 1].WData[ 0] =0x901a;
            DataUI[ 1].WData[ 1] =0x3a;
            DataUI[ 1].WData[ 2] =0x74;
            DataUI[ 1].WData[ 3] =0x6f;
            DataUI[ 1].WData[ 4] =0x6e;
            DataUI[ 1].WData[ 5] =0x67;
            DataUI[ 1].WData[ 6] =0;

            //------------------------------------------------------
            // Index 2
            //------------------------------------------------------
            DataUI[ 2].WData =new wchar_t[6 +1];
            DataUI[ 2].WData[ 0] =0x75db;
            DataUI[ 2].WData[ 1] =0x3a;
            DataUI[ 2].WData[ 2] =0x74;
            DataUI[ 2].WData[ 3] =0x6f;
            DataUI[ 2].WData[ 4] =0x6e;
            DataUI[ 2].WData[ 5] =0x67;
            DataUI[ 2].WData[ 6] =0;

            //------------------------------------------------------
            // Index 3
            //------------------------------------------------------
            DataUI[ 3].WData =new wchar_t[6 +1];
            DataUI[ 3].WData[ 0] =0x94dc;
            DataUI[ 3].WData[ 1] =0x3a;
            DataUI[ 3].WData[ 2] =0x74;
            DataUI[ 3].WData[ 3] =0x6f;
            DataUI[ 3].WData[ 4] =0x6e;
            DataUI[ 3].WData[ 5] =0x67;
            DataUI[ 3].WData[ 6] =0;

            //------------------------------------------------------
            // Index 4
            //------------------------------------------------------
            DataUI[ 4].WData =new wchar_t[6 +1];
            DataUI[ 4].WData[ 0] =0x7ae5;
            DataUI[ 4].WData[ 1] =0x3a;
            DataUI[ 4].WData[ 2] =0x74;
            DataUI[ 4].WData[ 3] =0x6f;
            DataUI[ 4].WData[ 4] =0x6e;
            DataUI[ 4].WData[ 5] =0x67;
            DataUI[ 4].WData[ 6] =0;

            //------------------------------------------------------
            // Index 5
            //------------------------------------------------------
            DataUI[ 5].WData =new wchar_t[6 +1];
            DataUI[ 5].WData[ 0] =0x6876;
            DataUI[ 5].WData[ 1] =0x3a;
            DataUI[ 5].WData[ 2] =0x74;
            DataUI[ 5].WData[ 3] =0x6f;
            DataUI[ 5].WData[ 4] =0x6e;
            DataUI[ 5].WData[ 5] =0x67;
            DataUI[ 5].WData[ 6] =0;

            //------------------------------------------------------
            // Index 6
            //------------------------------------------------------
            DataUI[ 6].WData =new wchar_t[6 +1];
            DataUI[ 6].WData[ 0] =0x7edf;
            DataUI[ 6].WData[ 1] =0x3a;
            DataUI[ 6].WData[ 2] =0x74;
            DataUI[ 6].WData[ 3] =0x6f;
            DataUI[ 6].WData[ 4] =0x6e;
            DataUI[ 6].WData[ 5] =0x67;
            DataUI[ 6].WData[ 6] =0;

            //------------------------------------------------------
            // Index 7
            //------------------------------------------------------
            DataUI[ 7].WData =new wchar_t[6 +1];
            DataUI[ 7].WData[ 0] =0x7b52;
            DataUI[ 7].WData[ 1] =0x3a;
            DataUI[ 7].WData[ 2] =0x74;
            DataUI[ 7].WData[ 3] =0x6f;
            DataUI[ 7].WData[ 4] =0x6e;
            DataUI[ 7].WData[ 5] =0x67;
            DataUI[ 7].WData[ 6] =0;

            //------------------------------------------------------
            // Index 8
            //------------------------------------------------------
            DataUI[ 8].WData =new wchar_t[6 +1];
            DataUI[ 8].WData[ 0] =0x6345;
            DataUI[ 8].WData[ 1] =0x3a;
            DataUI[ 8].WData[ 2] =0x74;
            DataUI[ 8].WData[ 3] =0x6f;
            DataUI[ 8].WData[ 4] =0x6e;
            DataUI[ 8].WData[ 5] =0x67;
            DataUI[ 8].WData[ 6] =0;

            //------------------------------------------------------
            // Index 9
            //------------------------------------------------------
            DataUI[ 9].WData =new wchar_t[6 +1];
            DataUI[ 9].WData[ 0] =0x6850;
            DataUI[ 9].WData[ 1] =0x3a;
            DataUI[ 9].WData[ 2] =0x74;
            DataUI[ 9].WData[ 3] =0x6f;
            DataUI[ 9].WData[ 4] =0x6e;
            DataUI[ 9].WData[ 5] =0x67;
            DataUI[ 9].WData[ 6] =0;

            //------------------------------------------------------
            // Index 10
            //------------------------------------------------------
            DataUI[10].WData =new wchar_t[6 +1];
            DataUI[10].WData[ 0] =0x77b3;
            DataUI[10].WData[ 1] =0x3a;
            DataUI[10].WData[ 2] =0x74;
            DataUI[10].WData[ 3] =0x6f;
            DataUI[10].WData[ 4] =0x6e;
            DataUI[10].WData[ 5] =0x67;
            DataUI[10].WData[ 6] =0;

            //------------------------------------------------------
            // Index 11
            //------------------------------------------------------
            DataUI[11].WData =new wchar_t[6 +1];
            DataUI[11].WData[ 0] =0x5f64;
            DataUI[11].WData[ 1] =0x3a;
            DataUI[11].WData[ 2] =0x74;
            DataUI[11].WData[ 3] =0x6f;
            DataUI[11].WData[ 4] =0x6e;
            DataUI[11].WData[ 5] =0x67;
            DataUI[11].WData[ 6] =0;

            //------------------------------------------------------
            // Index 12
            //------------------------------------------------------
            DataUI[12].WData =new wchar_t[6 +1];
            DataUI[12].WData[ 0] =0x4f5f;
            DataUI[12].WData[ 1] =0x3a;
            DataUI[12].WData[ 2] =0x74;
            DataUI[12].WData[ 3] =0x6f;
            DataUI[12].WData[ 4] =0x6e;
            DataUI[12].WData[ 5] =0x67;
            DataUI[12].WData[ 6] =0;

            //------------------------------------------------------
            // Index 13
            //------------------------------------------------------
            DataUI[13].WData =new wchar_t[6 +1];
            DataUI[13].WData[ 0] =0x916e;
            DataUI[13].WData[ 1] =0x3a;
            DataUI[13].WData[ 2] =0x74;
            DataUI[13].WData[ 3] =0x6f;
            DataUI[13].WData[ 4] =0x6e;
            DataUI[13].WData[ 5] =0x67;
            DataUI[13].WData[ 6] =0;

            //------------------------------------------------------
            // Index 14
            //------------------------------------------------------
            DataUI[14].WData =new wchar_t[6 +1];
            DataUI[14].WData[ 0] =0x783c;
            DataUI[14].WData[ 1] =0x3a;
            DataUI[14].WData[ 2] =0x74;
            DataUI[14].WData[ 3] =0x6f;
            DataUI[14].WData[ 4] =0x6e;
            DataUI[14].WData[ 5] =0x67;
            DataUI[14].WData[ 6] =0;

            //------------------------------------------------------
            // Index 15
            //------------------------------------------------------
            DataUI[15].WData =new wchar_t[6 +1];
            DataUI[15].WData[ 0] =0x6f7c;
            DataUI[15].WData[ 1] =0x3a;
            DataUI[15].WData[ 2] =0x74;
            DataUI[15].WData[ 3] =0x6f;
            DataUI[15].WData[ 4] =0x6e;
            DataUI[15].WData[ 5] =0x67;
            DataUI[15].WData[ 6] =0;

            //------------------------------------------------------
            // Index 16
            //------------------------------------------------------
            DataUI[16].WData =new wchar_t[6 +1];
            DataUI[16].WData[ 0] =0x4edd;
            DataUI[16].WData[ 1] =0x3a;
            DataUI[16].WData[ 2] =0x74;
            DataUI[16].WData[ 3] =0x6f;
            DataUI[16].WData[ 4] =0x6e;
            DataUI[16].WData[ 5] =0x67;
            DataUI[16].WData[ 6] =0;

            //------------------------------------------------------
            // Index 17
            //------------------------------------------------------
            DataUI[17].WData =new wchar_t[6 +1];
            DataUI[17].WData[ 0] =0x6078;
            DataUI[17].WData[ 1] =0x3a;
            DataUI[17].WData[ 2] =0x74;
            DataUI[17].WData[ 3] =0x6f;
            DataUI[17].WData[ 4] =0x6e;
            DataUI[17].WData[ 5] =0x67;
            DataUI[17].WData[ 6] =0;

            //------------------------------------------------------
            // Index 18
            //------------------------------------------------------
            DataUI[18].WData =new wchar_t[6 +1];
            DataUI[18].WData[ 0] =0x4f97;
            DataUI[18].WData[ 1] =0x3a;
            DataUI[18].WData[ 2] =0x74;
            DataUI[18].WData[ 3] =0x6f;
            DataUI[18].WData[ 4] =0x6e;
            DataUI[18].WData[ 5] =0x67;
            DataUI[18].WData[ 6] =0;

            //------------------------------------------------------
            // Index 19
            //------------------------------------------------------
            DataUI[19].WData =new wchar_t[6 +1];
            DataUI[19].WData[ 0] =0x55f5;
            DataUI[19].WData[ 1] =0x3a;
            DataUI[19].WData[ 2] =0x74;
            DataUI[19].WData[ 3] =0x6f;
            DataUI[19].WData[ 4] =0x6e;
            DataUI[19].WData[ 5] =0x67;
            DataUI[19].WData[ 6] =0;

            //------------------------------------------------------
            // Index 20
            //------------------------------------------------------
            DataUI[20].WData =new wchar_t[6 +1];
            DataUI[20].WData[ 0] =0x50ee;
            DataUI[20].WData[ 1] =0x3a;
            DataUI[20].WData[ 2] =0x74;
            DataUI[20].WData[ 3] =0x6f;
            DataUI[20].WData[ 4] =0x6e;
            DataUI[20].WData[ 5] =0x67;
            DataUI[20].WData[ 6] =0;

            //------------------------------------------------------
            // Index 21
            //------------------------------------------------------
            DataUI[21].WData =new wchar_t[6 +1];
            DataUI[21].WData[ 0] =0x5045;
            DataUI[21].WData[ 1] =0x3a;
            DataUI[21].WData[ 2] =0x74;
            DataUI[21].WData[ 3] =0x6f;
            DataUI[21].WData[ 4] =0x6e;
            DataUI[21].WData[ 5] =0x67;
            DataUI[21].WData[ 6] =0;

            //------------------------------------------------------
            // Index 22
            //------------------------------------------------------
            DataUI[22].WData =new wchar_t[6 +1];
            DataUI[22].WData[ 0] =0x7d71;
            DataUI[22].WData[ 1] =0x3a;
            DataUI[22].WData[ 2] =0x74;
            DataUI[22].WData[ 3] =0x6f;
            DataUI[22].WData[ 4] =0x6e;
            DataUI[22].WData[ 5] =0x67;
            DataUI[22].WData[ 6] =0;

            //------------------------------------------------------
            // Index 23
            //------------------------------------------------------
            DataUI[23].WData =new wchar_t[6 +1];
            DataUI[23].WData[ 0] =0x70b5;
            DataUI[23].WData[ 1] =0x3a;
            DataUI[23].WData[ 2] =0x74;
            DataUI[23].WData[ 3] =0x6f;
            DataUI[23].WData[ 4] =0x6e;
            DataUI[23].WData[ 5] =0x67;
            DataUI[23].WData[ 6] =0;

            //------------------------------------------------------
            // Index 24
            //------------------------------------------------------
            DataUI[24].WData =new wchar_t[6 +1];
            DataUI[24].WData[ 0] =0x5cd2;
            DataUI[24].WData[ 1] =0x3a;
            DataUI[24].WData[ 2] =0x74;
            DataUI[24].WData[ 3] =0x6f;
            DataUI[24].WData[ 4] =0x6e;
            DataUI[24].WData[ 5] =0x67;
            DataUI[24].WData[ 6] =0;

            //------------------------------------------------------
            // Index 25
            //------------------------------------------------------
            DataUI[25].WData =new wchar_t[6 +1];
            DataUI[25].WData[ 0] =0x833c;
            DataUI[25].WData[ 1] =0x3a;
            DataUI[25].WData[ 2] =0x74;
            DataUI[25].WData[ 3] =0x6f;
            DataUI[25].WData[ 4] =0x6e;
            DataUI[25].WData[ 5] =0x67;
            DataUI[25].WData[ 6] =0;

            //------------------------------------------------------
            // Index 26
            //------------------------------------------------------
            DataUI[26].WData =new wchar_t[6 +1];
            DataUI[26].WData[ 0] =0x578c;
            DataUI[26].WData[ 1] =0x3a;
            DataUI[26].WData[ 2] =0x74;
            DataUI[26].WData[ 3] =0x6f;
            DataUI[26].WData[ 4] =0x6e;
            DataUI[26].WData[ 5] =0x67;
            DataUI[26].WData[ 6] =0;

            //------------------------------------------------------
            // Index 27
            //------------------------------------------------------
            DataUI[27].WData =new wchar_t[6 +1];
            DataUI[27].WData[ 0] =0x9285;
            DataUI[27].WData[ 1] =0x3a;
            DataUI[27].WData[ 2] =0x74;
            DataUI[27].WData[ 3] =0x6f;
            DataUI[27].WData[ 4] =0x6e;
            DataUI[27].WData[ 5] =0x67;
            DataUI[27].WData[ 6] =0;

            //------------------------------------------------------
            // Index 28
            //------------------------------------------------------
            DataUI[28].WData =new wchar_t[6 +1];
            DataUI[28].WData[ 0] =0x66c8;
            DataUI[28].WData[ 1] =0x3a;
            DataUI[28].WData[ 2] =0x74;
            DataUI[28].WData[ 3] =0x6f;
            DataUI[28].WData[ 4] =0x6e;
            DataUI[28].WData[ 5] =0x67;
            DataUI[28].WData[ 6] =0;

            //------------------------------------------------------
            // Index 29
            //------------------------------------------------------
            DataUI[29].WData =new wchar_t[6 +1];
            DataUI[29].WData[ 0] =0x7ca1;
            DataUI[29].WData[ 1] =0x3a;
            DataUI[29].WData[ 2] =0x74;
            DataUI[29].WData[ 3] =0x6f;
            DataUI[29].WData[ 4] =0x6e;
            DataUI[29].WData[ 5] =0x67;
            DataUI[29].WData[ 6] =0;

            //------------------------------------------------------
            // Index 30
            //------------------------------------------------------
            DataUI[30].WData =new wchar_t[6 +1];
            DataUI[30].WData[ 0] =0x6a0b;
            DataUI[30].WData[ 1] =0x3a;
            DataUI[30].WData[ 2] =0x74;
            DataUI[30].WData[ 3] =0x6f;
            DataUI[30].WData[ 4] =0x6e;
            DataUI[30].WData[ 5] =0x67;
            DataUI[30].WData[ 6] =0;

            //------------------------------------------------------
            // Index 31
            //------------------------------------------------------
            DataUI[31].WData =new wchar_t[6 +1];
            DataUI[31].WData[ 0] =0x70d4;
            DataUI[31].WData[ 1] =0x3a;
            DataUI[31].WData[ 2] =0x74;
            DataUI[31].WData[ 3] =0x6f;
            DataUI[31].WData[ 4] =0x6e;
            DataUI[31].WData[ 5] =0x67;
            DataUI[31].WData[ 6] =0;

            //------------------------------------------------------
            // Index 32
            //------------------------------------------------------
            DataUI[32].WData =new wchar_t[6 +1];
            DataUI[32].WData[ 0] =0x56f2;
            DataUI[32].WData[ 1] =0x3a;
            DataUI[32].WData[ 2] =0x74;
            DataUI[32].WData[ 3] =0x6f;
            DataUI[32].WData[ 4] =0x6e;
            DataUI[32].WData[ 5] =0x67;
            DataUI[32].WData[ 6] =0;

            //------------------------------------------------------
            // Index 33
            //------------------------------------------------------
            DataUI[33].WData =new wchar_t[6 +1];
            DataUI[33].WData[ 0] =0x6d75;
            DataUI[33].WData[ 1] =0x3a;
            DataUI[33].WData[ 2] =0x74;
            DataUI[33].WData[ 3] =0x6f;
            DataUI[33].WData[ 4] =0x6e;
            DataUI[33].WData[ 5] =0x67;
            DataUI[33].WData[ 6] =0;

            //------------------------------------------------------
            // Index 34
            //------------------------------------------------------
            DataUI[34].WData =new wchar_t[6 +1];
            DataUI[34].WData[ 0] =0x6a66;
            DataUI[34].WData[ 1] =0x3a;
            DataUI[34].WData[ 2] =0x74;
            DataUI[34].WData[ 3] =0x6f;
            DataUI[34].WData[ 4] =0x6e;
            DataUI[34].WData[ 5] =0x67;
            DataUI[34].WData[ 6] =0;

            //------------------------------------------------------
            // Index 35
            //------------------------------------------------------
            DataUI[35].WData =new wchar_t[6 +1];
            DataUI[35].WData[ 0] =0x54c3;
            DataUI[35].WData[ 1] =0x3a;
            DataUI[35].WData[ 2] =0x74;
            DataUI[35].WData[ 3] =0x6f;
            DataUI[35].WData[ 4] =0x6e;
            DataUI[35].WData[ 5] =0x67;
            DataUI[35].WData[ 6] =0;

            //------------------------------------------------------
            // Index 36
            //------------------------------------------------------
            DataUI[36].WData =new wchar_t[6 +1];
            DataUI[36].WData[ 0] =0x75cc;
            DataUI[36].WData[ 1] =0x3a;
            DataUI[36].WData[ 2] =0x74;
            DataUI[36].WData[ 3] =0x6f;
            DataUI[36].WData[ 4] =0x6e;
            DataUI[36].WData[ 5] =0x67;
            DataUI[36].WData[ 6] =0;

            //------------------------------------------------------
            // Index 37
            //------------------------------------------------------
            DataUI[37].WData =new wchar_t[6 +1];
            DataUI[37].WData[ 0] =0x6e69;
            DataUI[37].WData[ 1] =0x3a;
            DataUI[37].WData[ 2] =0x74;
            DataUI[37].WData[ 3] =0x6f;
            DataUI[37].WData[ 4] =0x6e;
            DataUI[37].WData[ 5] =0x67;
            DataUI[37].WData[ 6] =0;

            //------------------------------------------------------
            // Index 38
            //------------------------------------------------------
            DataUI[38].WData =new wchar_t[6 +1];
            DataUI[38].WData[ 0] =0x6723;
            DataUI[38].WData[ 1] =0x3a;
            DataUI[38].WData[ 2] =0x74;
            DataUI[38].WData[ 3] =0x6f;
            DataUI[38].WData[ 4] =0x6e;
            DataUI[38].WData[ 5] =0x67;
            DataUI[38].WData[ 6] =0;

            //------------------------------------------------------
            // Index 39
            //------------------------------------------------------
            DataUI[39].WData =new wchar_t[6 +1];
            DataUI[39].WData[ 0] =0x8a77;
            DataUI[39].WData[ 1] =0x3a;
            DataUI[39].WData[ 2] =0x74;
            DataUI[39].WData[ 3] =0x6f;
            DataUI[39].WData[ 4] =0x6e;
            DataUI[39].WData[ 5] =0x67;
            DataUI[39].WData[ 6] =0;

            //------------------------------------------------------
            // Index 40
            //------------------------------------------------------
            DataUI[40].WData =new wchar_t[6 +1];
            DataUI[40].WData[ 0] =0x615f;
            DataUI[40].WData[ 1] =0x3a;
            DataUI[40].WData[ 2] =0x74;
            DataUI[40].WData[ 3] =0x6f;
            DataUI[40].WData[ 4] =0x6e;
            DataUI[40].WData[ 5] =0x67;
            DataUI[40].WData[ 6] =0;

            //------------------------------------------------------
            // Index 41
            //------------------------------------------------------
            DataUI[41].WData =new wchar_t[6 +1];
            DataUI[41].WData[ 0] =0x721e;
            DataUI[41].WData[ 1] =0x3a;
            DataUI[41].WData[ 2] =0x74;
            DataUI[41].WData[ 3] =0x6f;
            DataUI[41].WData[ 4] =0x6e;
            DataUI[41].WData[ 5] =0x67;
            DataUI[41].WData[ 6] =0;

            //------------------------------------------------------
            // Index 42
            //------------------------------------------------------
            DataUI[42].WData =new wchar_t[6 +1];
            DataUI[42].WData[ 0] =0x7d67;
            DataUI[42].WData[ 1] =0x3a;
            DataUI[42].WData[ 2] =0x74;
            DataUI[42].WData[ 3] =0x6f;
            DataUI[42].WData[ 4] =0x6e;
            DataUI[42].WData[ 5] =0x67;
            DataUI[42].WData[ 6] =0;

            //------------------------------------------------------
            // Index 43
            //------------------------------------------------------
            DataUI[43].WData =new wchar_t[6 +1];
            DataUI[43].WData[ 0] =0x729d;
            DataUI[43].WData[ 1] =0x3a;
            DataUI[43].WData[ 2] =0x74;
            DataUI[43].WData[ 3] =0x6f;
            DataUI[43].WData[ 4] =0x6e;
            DataUI[43].WData[ 5] =0x67;
            DataUI[43].WData[ 6] =0;

            //------------------------------------------------------
            // Index 44
            //------------------------------------------------------
            DataUI[44].WData =new wchar_t[6 +1];
            DataUI[44].WData[ 0] =0x8855;
            DataUI[44].WData[ 1] =0x3a;
            DataUI[44].WData[ 2] =0x74;
            DataUI[44].WData[ 3] =0x6f;
            DataUI[44].WData[ 4] =0x6e;
            DataUI[44].WData[ 5] =0x67;
            DataUI[44].WData[ 6] =0;

            //------------------------------------------------------
            // Index 45
            //------------------------------------------------------
            DataUI[45].WData =new wchar_t[6 +1];
            DataUI[45].WData[ 0] =0x81a7;
            DataUI[45].WData[ 1] =0x3a;
            DataUI[45].WData[ 2] =0x74;
            DataUI[45].WData[ 3] =0x6f;
            DataUI[45].WData[ 4] =0x6e;
            DataUI[45].WData[ 5] =0x67;
            DataUI[45].WData[ 6] =0;

            //------------------------------------------------------
            // Index 46
            //------------------------------------------------------
            DataUI[46].WData =new wchar_t[6 +1];
            DataUI[46].WData[ 0] =0x6c03;
            DataUI[46].WData[ 1] =0x3a;
            DataUI[46].WData[ 2] =0x74;
            DataUI[46].WData[ 3] =0x6f;
            DataUI[46].WData[ 4] =0x6e;
            DataUI[46].WData[ 5] =0x67;
            DataUI[46].WData[ 6] =0;

            //------------------------------------------------------
            // Index 47
            //------------------------------------------------------
            DataUI[47].WData =new wchar_t[6 +1];
            DataUI[47].WData[ 0] =0x9ba6;
            DataUI[47].WData[ 1] =0x3a;
            DataUI[47].WData[ 2] =0x74;
            DataUI[47].WData[ 3] =0x6f;
            DataUI[47].WData[ 4] =0x6e;
            DataUI[47].WData[ 5] =0x67;
            DataUI[47].WData[ 6] =0;

            //------------------------------------------------------
            // Index 48
            //------------------------------------------------------
            DataUI[48].WData =new wchar_t[6 +1];
            DataUI[48].WData[ 0] =0x72ea;
            DataUI[48].WData[ 1] =0x3a;
            DataUI[48].WData[ 2] =0x74;
            DataUI[48].WData[ 3] =0x6f;
            DataUI[48].WData[ 4] =0x6e;
            DataUI[48].WData[ 5] =0x67;
            DataUI[48].WData[ 6] =0;

            //------------------------------------------------------
            // Index 49
            //------------------------------------------------------
            DataUI[49].WData =new wchar_t[6 +1];
            DataUI[49].WData[ 0] =0x9256;
            DataUI[49].WData[ 1] =0x3a;
            DataUI[49].WData[ 2] =0x74;
            DataUI[49].WData[ 3] =0x6f;
            DataUI[49].WData[ 4] =0x6e;
            DataUI[49].WData[ 5] =0x67;
            DataUI[49].WData[ 6] =0;

            //------------------------------------------------------
            // Index 50
            //------------------------------------------------------
            DataUI[50].WData =new wchar_t[6 +1];
            DataUI[50].WData[ 0] =0x84ea;
            DataUI[50].WData[ 1] =0x3a;
            DataUI[50].WData[ 2] =0x74;
            DataUI[50].WData[ 3] =0x6f;
            DataUI[50].WData[ 4] =0x6e;
            DataUI[50].WData[ 5] =0x67;
            DataUI[50].WData[ 6] =0;

            //------------------------------------------------------
            // Index 51
            //------------------------------------------------------
            DataUI[51].WData =new wchar_t[6 +1];
            DataUI[51].WData[ 0] =0x9907;
            DataUI[51].WData[ 1] =0x3a;
            DataUI[51].WData[ 2] =0x74;
            DataUI[51].WData[ 3] =0x6f;
            DataUI[51].WData[ 4] =0x6e;
            DataUI[51].WData[ 5] =0x67;
            DataUI[51].WData[ 6] =0;

       };

       wchar_t* GetWDataSz(long Index)
       {
            return DataUI[Index].WData;
       }
};

#endif
