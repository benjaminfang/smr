//
//  SMR_plot.h
//  SMR_CPP
//
//  Created by Futao Zhang on 29/06/2016.
//  Copyright (c) 2016 Futao Zhang. All rights reserved.
//

#ifndef __SMR_CPP__SMR_plot__
#define __SMR_CPP__SMR_plot__

#include "SMR_data.h"
#include "SMR_data_p1.h"

namespace SMRDATA
{
    void plot_triple(char* outFileName, char* bFileName,char* gwasFileName, char* eqtlFileName,char* meqtlFileName, double maf,char* indilstName, char* snplstName,double p_hetero,double ld_top,int m_hetero , char* indilst2remove, char* snplst2exclde, double p_smr, char* refSNP, int cis_itvl, char* prbname, int prbWind,bool prbwindFlag, int snpchr, char* snprs, char* fromsnprs, char* tosnprs,int snpWind,int fromsnpkb, int tosnpkb,bool snpwindFlag,bool cis_flag, char* geneAnnoName, double pthres_me2esmr,double threshpsmrest);
}

#endif /* defined(__SMR_CPP__SMR_plot__) */
