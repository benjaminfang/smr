//
//  SMR_data.cpp
//  SRM_CPP
//
//  Created by Futao Zhang on 29/06/15.
//  Copyright (c) 2015 Futao Zhang. All rights reserved.
//

#include "SMR_data.h"
namespace SMRDATA
{
    int file_read_check(ifstream* in_file, const char* filename)
    {
        in_file->open(filename);
        if(!*in_file)  return 0;
        return 1;
    }
    void progress_print(float progress)
    {
        int barWidth = 70;
        cout << "[";
        int pos = barWidth * progress;
        for (int i = 0; i < barWidth; ++i)
        {
            if (i < pos) cout << "=";
            else if (i == pos) cout << ">";
            else cout << " ";
        }
        cout << "] " << int(progress * 100.0) << " %\r";
        cout.flush();    
    }

    void read_famfile(bInfo* bdata, string famfile) {
		bdata->_autosome_num = 22;
        ifstream Fam(famfile.c_str());
        if (!Fam) throw ("Error: can not open the file [" + famfile + "] to read.");
        cout << "Reading PLINK FAM file from [" + famfile + "]." << endl;
        
        int i = 0;
        string str_buf;
        bdata->_fid.clear();
        bdata->_pid.clear();
        bdata->_fa_id.clear();
        bdata->_mo_id.clear();
        bdata->_sex.clear();
        bdata->_pheno.clear();
        while (Fam) {
            Fam >> str_buf;
            if (Fam.eof()) break;
            bdata->_fid.push_back(str_buf);
            Fam >> str_buf;
            bdata->_pid.push_back(str_buf);
            Fam >> str_buf;
            bdata->_fa_id.push_back(str_buf);
            Fam >> str_buf;
            bdata->_mo_id.push_back(str_buf);
            Fam >> str_buf;
            bdata->_sex.push_back(atoi(str_buf.c_str()));
            Fam >> str_buf;
            bdata->_pheno.push_back(atoi(str_buf.c_str()));
        }
        Fam.clear();
        Fam.close();
        bdata->_indi_num = bdata->_fid.size();
        cout << bdata->_indi_num << " individuals to be included from [" + famfile + "]." << endl;
        
        // Initialize _keep
        bdata->_keep.clear();
        bdata->_keep.resize(bdata->_indi_num);
        bdata->_id_map.clear();
        int size = 0;
        for (int i = 0; i < bdata->_indi_num; i++) {
            bdata->_keep[i] = i;
            bdata->_id_map.insert(pair<string, int>(bdata->_fid[i] + ":" + bdata->_pid[i], i));
            if (size == bdata->_id_map.size()) throw ("Error: Duplicate individual ID found: \"" + bdata->_fid[i] + "\t" + bdata->_pid[i] + "\".");
            size = bdata->_id_map.size();
        }
    }
    
    void update_bim(bInfo* bdata,vector<int> &rsnp) {
        int i = 0;
        
        //update bim information
        vector<int> chr_buf, bp_buf;
        vector<char> a1_buf, a2_buf, ref_A_buf, other_A_buf;
        vector<string> snp_name_buf;
        vector<double> genet_dst_buf, impRsq_buf;
        for (i = 0; i < bdata->_snp_num; i++) {
            if (!rsnp[i]) continue;
            chr_buf.push_back(bdata->_chr[i]);
            snp_name_buf.push_back(bdata->_snp_name[i]);
            genet_dst_buf.push_back(bdata->_genet_dst[i]);
            bp_buf.push_back(bdata->_bp[i]);
            a1_buf.push_back(bdata->_allele1[i]);
            a2_buf.push_back(bdata->_allele2[i]);
            ref_A_buf.push_back(bdata->_ref_A[i]);
            other_A_buf.push_back(bdata->_other_A[i]);
            if(bdata->_impRsq.size()>0) impRsq_buf.push_back(bdata->_impRsq[i]);
        }
        bdata->_chr.clear();
        bdata->_snp_name.clear();
        bdata->_genet_dst.clear();
        bdata->_bp.clear();
        bdata->_allele1.clear();
        bdata->_allele2.clear();
        bdata->_ref_A.clear();
        bdata->_other_A.clear();
        bdata->_impRsq.clear();
        bdata->_chr = chr_buf;
        bdata->_snp_name = snp_name_buf;
        bdata->_genet_dst = genet_dst_buf;
        bdata->_bp = bp_buf;
        bdata->_allele1 = a1_buf;
        bdata->_allele2 = a2_buf;
        bdata->_ref_A = ref_A_buf;
        bdata->_other_A = other_A_buf;
        bdata->_impRsq=impRsq_buf;
        bdata->_snp_num = bdata->_chr.size();
        bdata->_include.clear();
        bdata-> _include.resize(bdata->_snp_num);
        bdata->_snp_name_map.clear();
        
        for (i = 0; i < bdata->_snp_num; i++) {
            bdata->_include[i] = i;
            bdata->_snp_name_map.insert(pair<string, int>(bdata->_snp_name[i], i));
        }
    }
    
    void update_fam(bInfo* bdata,vector<int> &rindi) {
        //update fam information
        int i = 0;
        vector<string> fid_buf, pid_buf, fa_id_buf, mo_id_buf;
        vector<int> sex_buf;
        vector<double> pheno_buf;
        for (i = 0; i < bdata->_indi_num; i++) {
            if (!rindi[i]) continue;
            fid_buf.push_back(bdata->_fid[i]);
            pid_buf.push_back(bdata->_pid[i]);
            fa_id_buf.push_back(bdata->_fa_id[i]);
            mo_id_buf.push_back(bdata->_mo_id[i]);
            sex_buf.push_back(bdata->_sex[i]);
            pheno_buf.push_back(bdata->_pheno[i]);
        }
        bdata->_fid.clear();
        bdata->_pid.clear();
        bdata->_fa_id.clear();
        bdata->_mo_id.clear();
        bdata->_sex.clear();
        bdata->_pheno.clear();
        bdata->_fid = fid_buf;
        bdata->_pid = pid_buf;
        bdata->_fa_id = fa_id_buf;
        bdata->_mo_id = mo_id_buf;
        bdata->_sex = sex_buf;
        bdata->_pheno = pheno_buf;
        
        bdata->_indi_num = bdata->_fid.size();
        bdata->_keep.clear();
        bdata->_keep.resize(bdata->_indi_num);
        bdata->_id_map.clear();
        for (i = 0; i < bdata->_indi_num; i++) {
            bdata->_keep[i] = i;
            bdata->_id_map.insert(pair<string, int>(bdata->_fid[i] + ":" + bdata->_pid[i], i));
        }
    }
    
	void update_epi(eqtlInfo* eqtlinfo)
	{
		eqtlinfo->_probNum = eqtlinfo->_include.size();

		vector<uint32_t> chr_buf, gd_buf,bp_buf;
		vector<string> prbID_buf, gene_buf;
		vector<char> orien_buf;
		for (int i = 0; i < eqtlinfo->_probNum; i++)
		{
			chr_buf.push_back(eqtlinfo->_epi_chr[eqtlinfo->_include[i]]);
			gd_buf.push_back(eqtlinfo->_epi_gd[eqtlinfo->_include[i]]);
			bp_buf.push_back(eqtlinfo->_epi_bp[eqtlinfo->_include[i]]);
			prbID_buf.push_back(eqtlinfo->_epi_prbID[eqtlinfo->_include[i]]);
			gene_buf.push_back(eqtlinfo->_epi_gene[eqtlinfo->_include[i]]);
			orien_buf.push_back(eqtlinfo->_epi_orien[eqtlinfo->_include[i]]);
		}
		eqtlinfo->_epi_chr.clear();
		eqtlinfo->_epi_gd.clear();
		eqtlinfo->_epi_bp.clear();
		eqtlinfo->_epi_prbID.clear();
		eqtlinfo->_epi_gene.clear();
		eqtlinfo->_epi_orien.clear();
		eqtlinfo->_epi_chr.swap(chr_buf);
		eqtlinfo->_epi_gd.swap(gd_buf);
		eqtlinfo->_epi_bp.swap(bp_buf);
		eqtlinfo->_epi_prbID.swap(prbID_buf);
		eqtlinfo->_epi_gene.swap(gene_buf);
		eqtlinfo->_epi_orien.swap(orien_buf);
        
        eqtlinfo->_include.clear();
        for (int i = 0; i < eqtlinfo->_probNum; i++) eqtlinfo->_include.push_back(i);

		/*		
		eqtlinfo->_epi_chr.assign(chr_buf.begin(),chr_buf.end());
		eqtlinfo->_epi_gd.assign(gd_buf.begin(), gd_buf.end());
		eqtlinfo->_epi_bp.assign(bp_buf.begin(), bp_buf.end());
		eqtlinfo->_epi_prbID.assign(prbID_buf.begin(), prbID_buf.end());
		eqtlinfo->_epi_gene.assign(gene_buf.begin(), gene_buf.end());
		eqtlinfo->_epi_orien.assign(orien_buf.begin(), orien_buf.end());
		*/
	}

	void update_esi(eqtlInfo* eqtlinfo)
	{
		eqtlinfo->_snpNum = eqtlinfo->_esi_include.size();	

		vector<uint32_t> chr_buf, gd_buf, bp_buf;
		vector<string> rs_buf;
		vector<char> allele1_buf, allele2_buf;
		for (int i = 0; i < eqtlinfo->_snpNum; i++)
		{
			chr_buf.push_back(eqtlinfo->_esi_chr[eqtlinfo->_esi_include[i]]);
			gd_buf.push_back(eqtlinfo->_esi_gd[eqtlinfo->_esi_include[i]]);
			bp_buf.push_back(eqtlinfo->_esi_bp[eqtlinfo->_esi_include[i]]);
			rs_buf.push_back(eqtlinfo->_esi_rs[eqtlinfo->_esi_include[i]]);
			allele1_buf.push_back(eqtlinfo->_esi_allele1[eqtlinfo->_esi_include[i]]);
			allele2_buf.push_back(eqtlinfo->_esi_allele2[eqtlinfo->_esi_include[i]]);
		}
		eqtlinfo->_esi_chr.clear();
		eqtlinfo->_esi_gd.clear();
		eqtlinfo->_esi_bp.clear();
		eqtlinfo->_esi_rs.clear();
		eqtlinfo->_esi_allele1.clear();
		eqtlinfo->_esi_allele2.clear();
		eqtlinfo->_esi_chr.swap(chr_buf);
		eqtlinfo->_esi_gd.swap(gd_buf);
		eqtlinfo->_esi_bp.swap(bp_buf);
		eqtlinfo->_esi_rs.swap(rs_buf);
		eqtlinfo->_esi_allele1.swap(allele1_buf);
		eqtlinfo->_esi_allele2.swap(allele2_buf);
        
        eqtlinfo->_esi_include.clear();
        eqtlinfo->_snp_name_map.clear();
        long size=0;
        for (int i = 0; i < eqtlinfo->_snpNum; i++)
        {
            eqtlinfo->_esi_include.push_back(i);
            /*
             // disabled currently
            eqtlinfo->_snp_name_map.insert(pair<string, int>(eqtlinfo->_esi_rs[i], i));
            if (size == eqtlinfo->_snp_name_map.size()) throw ("Error: Duplicated SNP IDs found: \"" + eqtlinfo->_esi_rs[i] + "\".");
            size = eqtlinfo->_snp_name_map.size();
             */
        }
	}

    void read_bimfile(bInfo* bdata,string bimfile) {
        // Read bim file: recombination rate is defined between SNP i and SNP i-1
        int ibuf = 0;
        string cbuf = "0";
        double dbuf = 0.0;
        string str_buf;
        ifstream Bim(bimfile.c_str());
        if (!Bim) throw ("Error: can not open the file [" + bimfile + "] to read.");
        cout << "Reading PLINK BIM file from [" + bimfile + "]." << endl;
        bdata->_chr.clear();
        bdata->_snp_name.clear();
        bdata->_genet_dst.clear();
        bdata->_bp.clear();
        bdata->_allele1.clear();
        bdata->_allele2.clear();
        while (Bim) {
            Bim >> ibuf;
            if (Bim.eof()) break;
            bdata->_chr.push_back(ibuf);
            Bim >> str_buf;
            bdata->_snp_name.push_back(str_buf);
            Bim >> dbuf;
            bdata->_genet_dst.push_back(dbuf);
            Bim >> ibuf;
            bdata->_bp.push_back(ibuf);
            Bim >> cbuf;
            StrFunc::to_upper(cbuf);
            bdata->_allele1.push_back(cbuf.c_str()[0]);
            Bim >> cbuf;
            StrFunc::to_upper(cbuf);
            bdata->_allele2.push_back(cbuf.c_str()[0]);
        }
        Bim.close();
        bdata->_snp_num = bdata->_chr.size();
        bdata->_ref_A = bdata->_allele1;
        bdata->_other_A = bdata->_allele2;
        cout << bdata->_snp_num << " SNPs to be included from [" + bimfile + "]." << endl;
        
        // Initialize _include
         bdata->_include.clear();
         bdata->_include.resize( bdata->_snp_num);
         bdata->_snp_name_map.clear();
        int size = 0;
        for (int i = 0; i <  bdata->_snp_num; i++) {
             bdata->_include[i] = i;
             bdata->_snp_name_map.insert(pair<string, int>( bdata->_snp_name[i], i));
            if (size ==  bdata->_snp_name_map.size()) throw ("Error: Duplicated SNP IDs found: \"" +  bdata->_snp_name[i] + "\".");
            size =  bdata->_snp_name_map.size();
        }
    }
    // some code are adopted from PLINK with modifications
    void read_bedfile(bInfo* bdata, string bedfile)
    {
        int i = 0, j = 0, k = 0;
        
        // Flag for reading individuals and SNPs
        vector<int> rindi, rsnp;
        //get_rindi
        rindi.clear();
        rindi.resize(bdata->_indi_num);
        for (int i = 0; i < bdata->_indi_num; i++) {
            if (bdata->_id_map.find(bdata->_fid[i] + ":" + bdata->_pid[i]) != bdata->_id_map.end()) rindi[i] = 1;
            else rindi[i] = 0;
        }
        //get_rsnp
        rsnp.clear();
        rsnp.resize(bdata->_snp_num);
        for (int i = 0; i < bdata->_snp_num; i++) {
            if (bdata->_snp_name_map.find(bdata->_snp_name[i]) != bdata->_snp_name_map.end()) rsnp[i] = 1;
            else rsnp[i] = 0;
        }
        
        if (bdata->_include.size() == 0) throw ("Error: No SNP is retained for analysis.");
        if (bdata->_keep.size() == 0) throw ("Error: No individual is retained for analysis.");
        
        // Read bed file
        char ch[1];
        bitset<8> b;
        bdata->_snp_1.resize(bdata->_include.size());
        bdata->_snp_2.resize(bdata->_include.size());
        for (i = 0; i < bdata->_include.size(); i++) {
			bdata->_snp_1[i].reserve(bdata->_keep.size());
			bdata->_snp_2[i].reserve(bdata->_keep.size());
        }
        fstream BIT(bedfile.c_str(), ios::in | ios::binary);
        if (!BIT) throw ("Error: can not open the file [" + bedfile + "] to read.");
        cout << "Reading PLINK BED file from [" + bedfile + "] in SNP-major format ..." << endl;
        for (i = 0; i < 3; i++) BIT.read(ch, 1); // skip the first three bytes
        int snp_indx = 0, indi_indx = 0;
        for (j = 0, snp_indx = 0; j < bdata->_snp_num; j++) { // Read genotype in SNP-major mode, 00: homozygote AA; 11: homozygote BB; 01: hetezygote; 10: missing
            if (!rsnp[j]) {
                for (i = 0; i < bdata->_indi_num; i += 4) BIT.read(ch, 1);
                continue;
            }
            for (i = 0, indi_indx = 0; i < bdata->_indi_num;) {
                BIT.read(ch, 1);
                if (!BIT) throw ("Error: problem with the BED file ... has the FAM/BIM file been changed?");
                b = ch[0];                
                k = 0;
                while (k < 7 && i < bdata->_indi_num) { // change code: 11 for AA; 00 for BB;
                    if (!rindi[i]) k += 2;
                    else {						
                        bdata->_snp_2[snp_indx][indi_indx] = (!b[k++]);
                        bdata->_snp_1[snp_indx][indi_indx] = (!b[k++]);
                        indi_indx++;
                    }
                    i++;
                }
            }
            if (snp_indx == bdata->_include.size()) break;
            snp_indx++;
        }
        BIT.clear();
        BIT.close();
        cout << "Genotype data for " << bdata->_keep.size() << " individuals and " << bdata->_include.size() << " SNPs to be included from [" + bedfile + "]." << endl;
        
        update_fam(bdata, rindi);
        update_bim(bdata, rsnp);
    }
    


    void read_gwas_data(gwasData* gdata, char* gwasFileName)
    {
        ifstream gwasFile;
        if(!file_read_check(&gwasFile, gwasFileName))
        {
            
            fprintf (stderr, "%s: Couldn't open file %s\n",
                     gwasFileName, strerror (errno));
            exit (EXIT_FAILURE);
        }
        cout << "Reading GWAS summary-level statistics from [" + string(gwasFileName) + "]." << endl;
        char buf[MAX_LINE_SIZE];
        int lineNum(0);
        gwasFile.getline(buf,MAX_LINE_SIZE);// the header
        while(!gwasFile.eof())
        {
            gwasFile.getline(buf,MAX_LINE_SIZE);
            lineNum++;
        }
        if(buf[0]=='\0') lineNum--;
        
        gdata->snpNum=lineNum;
        gdata->snpName.resize(lineNum);
        gdata->allele_1=(char*)malloc(lineNum*sizeof(char));
        gdata->allele_2=(char*)malloc(lineNum*sizeof(char));
        gdata->freq=(double*)malloc(lineNum*sizeof(double));
        gdata->byz=(double*)malloc(lineNum*sizeof(double));
        gdata->seyz=(double*)malloc(lineNum*sizeof(double));
        gdata->pvalue=(double*)malloc(lineNum*sizeof(double));
        gdata->splSize=(uint32_t*)malloc(lineNum*sizeof(uint32_t));
        gdata->_include.resize(lineNum);
        
        gwasFile.clear(ios::goodbit);
        gwasFile.seekg (0, ios::beg);
        gwasFile.getline(buf,MAX_LINE_SIZE);
        for(int i=0;i<lineNum;i++)
        {
            string tmpStr;
            gwasFile.getline(buf,MAX_LINE_SIZE);
            istringstream iss(buf);
            iss>>tmpStr;
            gdata->snpName[i]=tmpStr;
            iss>>tmpStr;
            memcpy(&gdata->allele_1[i],tmpStr.c_str(),1);
            iss>>tmpStr;
            memcpy(&gdata->allele_2[i],tmpStr.c_str(),1);
            iss>>tmpStr;
            gdata->freq[i]=atof(tmpStr.c_str());
            iss>>tmpStr;
            gdata->byz[i]=atof(tmpStr.c_str());
            iss>>tmpStr;
            gdata->seyz[i]=atof(tmpStr.c_str());
            iss>>tmpStr;
            gdata->pvalue[i]=atof(tmpStr.c_str());
            iss>>tmpStr;
            gdata->splSize[i]=atoi(tmpStr.c_str());
            gdata->_include[i]=i;
        }
        to_upper(gdata->allele_1,lineNum);
        to_upper(gdata->allele_2,lineNum);
       cout <<"GWAS summary statistics of "<<gdata->snpNum << " SNPs to be included from [" + string(gwasFileName) + "]." << endl;
        gwasFile.close();
    }
	/*
    void update_include_map(eqtlInfo* eqtlinfo)
    {
        eqtlinfo->_incld_id_map.clear();
        long size=0;
        for(int i=0;i<eqtlinfo->_esi_include.size();i++)
        {
            eqtlinfo->_incld_id_map.insert(pair<int,int>(eqtlinfo->_esi_include[i],i));
            if (size == eqtlinfo->_incld_id_map.size()) throw ("Error: Duplicated SNP IDs found: \"" + eqtlinfo->_esi_rs[eqtlinfo->_esi_include[i]] + "\".");
            size = eqtlinfo->_incld_id_map.size();
        }
        
    }
	*/
    void read_esifile(eqtlInfo* eqtlinfo, string esifile)
    {
        ifstream esi(esifile.c_str());
        if (!esi) throw ("Error: can not open the file [" + esifile + "] to read.");
        cout << "Reading eQTL SNP information from [" + esifile + "]." << endl;
        eqtlinfo->_esi_chr.clear();
        eqtlinfo->_esi_rs.clear();
        eqtlinfo->_esi_gd.clear();
        eqtlinfo->_esi_bp.clear();
        eqtlinfo->_esi_allele1.clear();
        eqtlinfo->_esi_allele2.clear();
		eqtlinfo->_esi_include.clear();

        char buf[MAX_LINE_SIZE];
        int lineNum(0);
        while(!esi.eof())
        {
            esi.getline(buf,MAX_LINE_SIZE);
            lineNum++;
        }
        if(buf[0]=='\0') lineNum--;
        eqtlinfo->_snpNum=lineNum;        
        cout << eqtlinfo->_snpNum << " SNPs to be included from [" + esifile + "]." << endl;
        
        eqtlinfo->_esi_chr.resize(lineNum);
        eqtlinfo->_esi_rs.resize(lineNum);
        eqtlinfo->_esi_gd.resize(lineNum);
        eqtlinfo->_esi_bp.resize(lineNum);
        eqtlinfo->_esi_allele1.resize(lineNum);
        eqtlinfo->_esi_allele2.resize(lineNum);
		eqtlinfo->_esi_include.resize(lineNum);
        esi.clear(ios::goodbit);
        esi.seekg (0, ios::beg);
        long size=0;
        for(int i=0;i<lineNum;i++)
        {
            string tmpStr;
            esi.getline(buf,MAX_LINE_SIZE);
            istringstream iss(buf);
            iss>>tmpStr;
            eqtlinfo->_esi_chr[i]=atoi(tmpStr.c_str());
            iss>>tmpStr;
            eqtlinfo->_esi_rs[i]=tmpStr;
            /*
             //disabled currently
            eqtlinfo->_snp_name_map.insert(pair<string, int>(tmpStr, i));
            if (size == eqtlinfo->_snp_name_map.size()) throw ("Error: Duplicated SNP IDs found: \"" + tmpStr + "\".");
            size = eqtlinfo->_snp_name_map.size();
            */
            iss>>tmpStr;
            eqtlinfo->_esi_gd[i]=atoi(tmpStr.c_str());
            iss>>tmpStr;
            eqtlinfo->_esi_bp[i]=atoi(tmpStr.c_str());
            iss>>tmpStr;
            StrFunc::to_upper(tmpStr);
            eqtlinfo->_esi_allele1[i]=tmpStr.c_str()[0];
            iss>>tmpStr;
            StrFunc::to_upper(tmpStr);
            eqtlinfo->_esi_allele2[i]=tmpStr.c_str()[0];
			eqtlinfo->_esi_include[i] = i;
            
        }       
        esi.close();
       
    }
    void read_epifile(eqtlInfo* eqtlinfo, string epifile)
    {
        ifstream epi(epifile.c_str());
        if (!epi) throw ("Error: can not open the file [" + epifile + "] to read.");
        cout << "Reading eQTL probe information from [" + epifile + "]." << endl;
        eqtlinfo->_epi_chr.clear();
        eqtlinfo->_epi_prbID.clear();
        eqtlinfo->_epi_gd.clear();
        eqtlinfo->_epi_bp.clear();
        eqtlinfo->_epi_gene.clear();
        eqtlinfo->_epi_orien.clear();
		eqtlinfo->_include.clear();

        char buf[MAX_LINE_SIZE];
        int lineNum(0);
        while(!epi.eof())
        {
            epi.getline(buf,MAX_LINE_SIZE);
            lineNum++;
        }
        if(buf[0]=='\0') lineNum--;
        eqtlinfo->_probNum=lineNum;
        cout << eqtlinfo->_probNum << " Probes to be included from [" + epifile + "]." << endl;
        
        eqtlinfo->_epi_chr.resize(lineNum);
        eqtlinfo->_epi_prbID.resize(lineNum);
        eqtlinfo->_epi_gd.resize(lineNum);
        eqtlinfo->_epi_bp.resize(lineNum);
        eqtlinfo->_epi_gene.resize(lineNum);
        eqtlinfo->_epi_orien.resize(lineNum);
        eqtlinfo->_include.resize(lineNum);
        epi.clear(ios::goodbit);
        epi.seekg (0, ios::beg);
        for(int i=0;i<lineNum;i++)
        {
            string tmpStr;
            epi.getline(buf,MAX_LINE_SIZE);
            istringstream iss(buf);
            iss>>tmpStr;
            eqtlinfo->_epi_chr[i]=atoi(tmpStr.c_str());
            iss>>tmpStr;
            eqtlinfo->_epi_prbID[i]=tmpStr;
            iss>>tmpStr;
            eqtlinfo->_epi_gd[i]=atoi(tmpStr.c_str());
            iss>>tmpStr;
            eqtlinfo->_epi_bp[i]=atoi(tmpStr.c_str());
            iss>>tmpStr;
            eqtlinfo->_epi_gene[i]=tmpStr.c_str();
            iss>>tmpStr;
            eqtlinfo->_epi_orien[i]=tmpStr.c_str()[0];
            eqtlinfo->_include[i]=i;
        }
        
        epi.close();
    }    
    
    void read_besdfile(eqtlInfo* eqtlinfo, string besdfile)
    {
        if (eqtlinfo->_include.size() == 0) throw ("Error: No probe is retained for analysis.");
        if (eqtlinfo->_esi_include.size() == 0) throw ("Error: No SNP is retained for analysis.");
        
        // the fastest way is using malloc and memcpy
        char buf[MAX_LINE_SIZE+4];
        ifstream besd(besdfile.c_str(), ios::in|ios::binary);
        if(!besd)
        {
            fprintf (stderr, "%s: Couldn't open file %s\n",
                     besdfile.c_str(), strerror (errno));
            exit (EXIT_FAILURE);
        }
        cout << "Reading eQTL summary-level statistics from [" + besdfile + "]." << endl;
        
        besd.read(buf, 4);
        float* flag;
        flag=(float *)buf;
        if((int)*flag == SPARSE_FILE_TYPE_2){
            // clear datastruct for dense befor read sparse
            eqtlinfo->_bxz.clear();
            eqtlinfo->_sexz.clear();
            
            uint64_t colNum=(eqtlinfo->_probNum<<1)+1;
            uint64_t valNum;
            uint64_t lSize;
            char* buffer;
            besd.seekg(0,besd.end);
            lSize = besd.tellg();
            
            besd.seekg(4); // same as besd.seekg(4, besd.beg);
            besd.read(buf, sizeof(uint64_t));
            valNum=*(uint64_t *)buf;
            if( lSize - (sizeof(float) + sizeof(uint64_t) + (colNum+valNum)*sizeof(uint32_t) + valNum*sizeof(float)) != 0) {fputs ("wrong element number",stderr); exit (3);}
            
            
            buffer = (char*) malloc (sizeof(char)*(lSize));
            if (buffer == NULL) {fputs ("Memory error",stderr); exit (1);}
            besd.read(buffer,lSize);
            if (besd.gcount()+sizeof(float) + sizeof(uint64_t) != lSize) {fputs ("Reading error",stderr); exit (2);}
            
            
            uint32_t* ptr;
            ptr=(uint32_t *)buffer;
            
            if(eqtlinfo->_include.size()<eqtlinfo->_probNum || eqtlinfo->_esi_include.size()<eqtlinfo->_snpNum)
            {
                eqtlinfo->_cols.resize((eqtlinfo->_include.size()<<1)+1);
                eqtlinfo->_cols[0]=*ptr;
                uint32_t* row_ptr;
                row_ptr=ptr+colNum;
                float* val_ptr;
                val_ptr=(float*)(row_ptr+valNum);
                
                map<int, int > _incld_id_map;
                long size = 0;
                for (int i = 0; i<eqtlinfo->_esi_include.size(); i++)
                {
                    _incld_id_map.insert(pair<int, int>(eqtlinfo->_esi_include[i], i));
                    if (size == _incld_id_map.size()) throw ("Error: Duplicated SNP IDs found: \"" + eqtlinfo->_esi_rs[eqtlinfo->_esi_include[i]] + "\".");
                    size = _incld_id_map.size();
                }

                for(int i=0;i<eqtlinfo->_include.size();i++)
                {
                    uint32_t pid=eqtlinfo->_include[i];
                    uint32_t pos=*(ptr+(pid<<1));
                    uint32_t pos1=*(ptr+(pid<<1)+1);
                    uint32_t num=pos1-pos;
                    uint32_t real_num=0;
                    for(int j=0;j<num<<1;j++)
                    {
                        uint32_t rid=*(row_ptr+pos+j);
                        
                        map<int, int>::iterator iter;
                        iter=_incld_id_map.find(rid);
                        if(iter!=_incld_id_map.end())
                        {
                            int sid=iter->second;
                        
                       // long sid=find(eqtlinfo->_esi_include.begin(),eqtlinfo->_esi_include.end(),rid)-eqtlinfo->_esi_include.begin(); //slow
                      //  if(sid<eqtlinfo->_esi_include.size())
                      //  {
                            eqtlinfo->_rowid.push_back(sid);
                            eqtlinfo->_val.push_back(*(val_ptr+pos+j));
                            real_num++;
                        }
                       
                    }
                    eqtlinfo->_cols[(i<<1)+1]=(real_num>>1)+eqtlinfo->_cols[i<<1];
                    eqtlinfo->_cols[i+1<<1]=real_num+eqtlinfo->_cols[i<<1];
                }
                eqtlinfo->_valNum = eqtlinfo->_val.size();
                cout<<"eQTL summary-level statistics of "<<eqtlinfo->_include.size()<<" Probes and "<<eqtlinfo->_esi_include.size()<<" SNPs to be included from [" + besdfile + "]." <<endl;
                if(eqtlinfo->_include.size()<eqtlinfo->_probNum ) update_epi(eqtlinfo);
                if(eqtlinfo->_esi_include.size()<eqtlinfo->_snpNum) update_esi(eqtlinfo);
            }
            else
            {
                eqtlinfo->_cols.resize(colNum);
                eqtlinfo->_rowid.resize(valNum);
                eqtlinfo->_val.resize(valNum);
                
                for(int i=0;i<colNum;i++) eqtlinfo->_cols[i]=*ptr++;
                for(int i=0;i<valNum;i++) eqtlinfo->_rowid[i]=*ptr++;
                float* val_ptr=(float*)ptr;
                for(int i=0;i<valNum;i++) eqtlinfo->_val[i]=*val_ptr++;
                eqtlinfo->_valNum = valNum;
                cout<<"eQTL summary-level statistics of "<<eqtlinfo->_probNum<<" Probes and "<<eqtlinfo->_snpNum<<" SNPs to be included from [" + besdfile + "]." <<endl;
            }
            // terminate
            free (buffer);
        }
        else if((int)*flag == DENSE_FILE_TYPE_1 )
        {
            // clear datastruct for sparse befor read dense
            eqtlinfo->_cols.clear();
            eqtlinfo->_rowid.clear();
            eqtlinfo->_val.clear();
            eqtlinfo->_valNum = 0;
            
            eqtlinfo->_bxz.resize(eqtlinfo->_include.size());
            eqtlinfo->_sexz.resize(eqtlinfo->_include.size());
            for(unsigned int i=0;i<eqtlinfo->_include.size();i++)
            {
                eqtlinfo->_bxz[i].resize(eqtlinfo->_esi_include.size());
                eqtlinfo->_sexz[i].resize(eqtlinfo->_esi_include.size());
            }
            char* buffer;
            buffer = (char*) malloc (sizeof(char)*eqtlinfo->_snpNum<<3);
            if (buffer == NULL) {fputs ("Memory error",stderr); exit (1);}
            float* ft;
            float* se_ptr;
            if (eqtlinfo->_include.size()<eqtlinfo->_probNum || eqtlinfo->_esi_include.size()<eqtlinfo->_snpNum)  //means with the parameter --extract-probe. This also can read all the probes, but currently I don't think it is good for too many I/Os.
            {
                for(int i=0;i<eqtlinfo->_include.size();i++)
                {
                    unsigned long pid=eqtlinfo->_include[i];
                    besd.seekg(((pid<<1)*eqtlinfo->_snpNum+1)<<2);
                    memset(buffer,0,sizeof(char)*eqtlinfo->_snpNum<<3);
                    besd.read(buffer,eqtlinfo->_snpNum<<3);
                    ft=(float *)buffer;
                    for (int j = 0; j<eqtlinfo->_esi_include.size(); j++) eqtlinfo->_bxz[i][j] = *(ft + eqtlinfo->_esi_include[j]);
                    se_ptr = ft + eqtlinfo->_snpNum;
                    for (int j = 0; j<eqtlinfo->_esi_include.size(); j++) eqtlinfo->_sexz[i][j] = *(se_ptr + eqtlinfo->_esi_include[j]);
                }
                std::cout << "eQTL summary-level statistics of " << eqtlinfo->_include.size() << " Probes and " << eqtlinfo->_esi_include.size() << " SNPs to be included from [" + besdfile + "]." << endl;
                if(eqtlinfo->_include.size()<eqtlinfo->_probNum ) update_epi(eqtlinfo);
                if(eqtlinfo->_esi_include.size()<eqtlinfo->_snpNum) update_esi(eqtlinfo);
            }
            else
            {
                //without --extract-probe, read with less I/O. and need not to update epi.
                //read with static buffer. If dynamic buffer, 2GB per I/O can be more efficient.
                /*
                 unsigned long long count=0;
                 while(!besd.eof())
                 {
                 besd.read(buf,MAX_LINE_NUM);
                 unsigned long Bread=besd.gcount();
                 buf[Bread]='\0';
                 char* ptr=buf;
                 //while(*ptr != '\0') //can not use this, too many 0x00 in buf
                 while(Bread)
                 {
                 unsigned long pid=count/eqtlinfo->_snpNum;
                 unsigned long sid=count++%eqtlinfo->_snpNum;
                 ft=(float *)ptr;
                 if(pid&1) eqtlinfo->_sexz[pid>>1][sid]=*ft;
                 else eqtlinfo->_bxz[pid>>1][sid]=*ft;
                 ptr+=4;
                 Bread-=4;
                 }
                 }
                 cout<<"eQTL summary-level statistics of "<<eqtlinfo->_probNum<<" Probes and "<<eqtlinfo->_snpNum<<" SNPs to be included from [" + besdfile + "]." <<endl;
                 */
                
                /*
                 
                 // besd.seekg(0,besd.end);
                 // uint64_t lSize = besd.tellg();
                 //  lSize-=4;
                 //  besd.seekg(4); // same as besd.seekg(4, besd.beg);
                 uint64_t alread=0;
                 
                 char* buff;
                 uint64_t buffszie=0x40000000;
                 buff = (char*) malloc (sizeof(char)*buffszie);
                 if (buff == NULL) {fputs ("Memory error",stderr); exit (1);}
                 memset(buff,0,sizeof(char)*buffszie);
                 uint64_t count=0;
                 while(!besd.eof())
                 {
                 besd.read(buff,buffszie);
                 unsigned long Bread=besd.gcount();
                 alread+=Bread;
                 char* ptr=buff;
                 while(Bread)
                 {
                 unsigned long pid=count/eqtlinfo->_snpNum;
                 unsigned long sid=count++%eqtlinfo->_snpNum;
                 ft=(float *)ptr;
                 if(pid&1) eqtlinfo->_sexz[pid>>1][sid]=*ft;
                 else eqtlinfo->_bxz[pid>>1][sid]=*ft;
                 ptr+=4;
                 Bread-=4;
                 }
                 cout<<alread<<":"<<(alread>>30)<<"GB "<<endl;
                 }
                 cout<<"eQTL summary-level statistics of "<<eqtlinfo->_probNum<<" Probes and "<<eqtlinfo->_snpNum<<" SNPs to be included from [" + besd
                 file + "]." <<endl;
                 free(buff);
                 */
                
                /*
                char* buff;
                uint64_t buffszie=0x40000000;
                buff = (char*) malloc (sizeof(char)*buffszie);
                if (buff == NULL) {fputs ("Memory error",stderr); exit (1);}
                memset(buff,0,sizeof(char)*buffszie);
                
                uint64_t perbeta=(eqtlinfo->_snpNum<<2);
                uint64_t probonce=sizeof(char)*buffszie/perbeta;  //should be even number
                probonce>>=1;
                probonce<<=1;
                uint64_t readsize=perbeta*probonce;
                uint64_t probcount=0;
                while(!besd.eof())
                {
                    besd.read(buff,readsize);
                    unsigned long Bread=besd.gcount();
                    float* rptr=(float *)buff;
                    while(Bread)
                    {
                        memcpy(&eqtlinfo->_bxz[probcount][0],rptr,perbeta);
                        rptr+=eqtlinfo->_snpNum;
                        memcpy(&eqtlinfo->_sexz[probcount++][0],rptr,perbeta);
                        rptr+=eqtlinfo->_snpNum;
                        Bread-=(perbeta<<1);
                    }
                     cout<<probcount<<" done! "<<endl;
                }
                cout<<"eQTL summary-level statistics of "<<eqtlinfo->_probNum<<" Probes and "<<eqtlinfo->_snpNum<<" SNPs to be included from [" + besdfile + "]." <<endl;
                free(buff);
                 */
                
                char* buff;
                uint64_t buffszie=0x40000000;
                buff = (char*) malloc (sizeof(char)*buffszie);
                if (buff == NULL) {fputs ("Memory error",stderr); exit (1);}
                memset(buff,0,sizeof(char)*buffszie);
                
                uint64_t perbeta=(eqtlinfo->_snpNum<<2);
                uint64_t probonce=sizeof(char)*buffszie/perbeta;  //should be even number
                probonce>>=1;
                probonce<<=1;
                uint64_t readsize=perbeta*probonce;
                uint64_t probcount=0;
                while(!besd.eof())
                {
                    besd.read(buff,readsize);
                    uint64_t Bread=besd.gcount();
                    char* rptr=buff;
                    while(Bread)
                    {
                        memcpy(&eqtlinfo->_bxz[probcount][0],rptr,perbeta);
                        rptr+=perbeta;
                        memcpy(&eqtlinfo->_sexz[probcount++][0],rptr,perbeta);
                        rptr+=perbeta;
                        Bread-=(perbeta<<1);
                    }
                    printf("Redinging... %3.0f%%\r", 100.0*probcount/eqtlinfo->_probNum);
                    fflush(stdout);
                }
                cout<<"\neQTL summary-level statistics of "<<eqtlinfo->_probNum<<" Probes and "<<eqtlinfo->_snpNum<<" SNPs to be included from [" + besdfile + "]." <<endl;
                free(buff);

            }
            free(buffer);
        }
        else if ((int)*flag == SPARSE_FILE_TYPE_1 )
        {
            // clear datastruct for dense befor read sparse
            eqtlinfo->_bxz.clear();
            eqtlinfo->_sexz.clear();
            
            uint64_t colNum=eqtlinfo->_probNum<<1;
            uint64_t valNum;
            uint64_t lSize;
            char* buffer;
            besd.seekg(0,besd.end);
            lSize = besd.tellg();
            
            besd.seekg(4); // same as besd.seekg(4, besd.beg);
            besd.read(buf, 4);
            valNum=(uint32_t)*(float *)buf; // int to float then float to int back can lose pricision. hence this clause and bellow are unbelievable
             if(lSize-((3+colNum+(valNum<<1))<<2) != 0) {fputs ("wrong element number",stderr); exit (3);}
            
            valNum=((lSize>>2)-3-colNum)>>1;
            
            buffer = (char*) malloc (sizeof(char)*(lSize-8));
            if (buffer == NULL) {fputs ("Memory error",stderr); exit (1);}
            besd.read(buffer,(lSize-8));
            if (besd.gcount()+8 != lSize) {fputs ("Reading error",stderr); exit (2);}
            float* ptr;
            ptr=(float *)buffer;
            
            if(eqtlinfo->_include.size()<eqtlinfo->_probNum || eqtlinfo->_esi_include.size()<eqtlinfo->_snpNum)
            {                
                eqtlinfo->_cols.resize((eqtlinfo->_include.size()<<1)+1);
                eqtlinfo->_cols[0]=(uint32_t)*ptr;
                float* row_ptr;
                row_ptr=ptr+colNum+1;
                float* val_ptr;
                val_ptr=row_ptr+valNum;

				map<int, int > _incld_id_map;
				long size = 0;
				for (int i = 0; i<eqtlinfo->_esi_include.size(); i++)
				{
					_incld_id_map.insert(pair<int, int>(eqtlinfo->_esi_include[i], i));
					if (size == _incld_id_map.size()) throw ("Error: Duplicated SNP IDs found: \"" + eqtlinfo->_esi_rs[eqtlinfo->_esi_include[i]] + "\".");
					size = _incld_id_map.size();
				}

                for(int i=0;i<eqtlinfo->_include.size();i++)
                {
                    unsigned long pid=eqtlinfo->_include[i];
                    uint32_t pos=(uint32_t)*(ptr+(pid<<1));
                    uint32_t pos1=(uint32_t)*(ptr+(pid<<1)+1);
                    uint32_t num=pos1-pos;
                    uint32_t real_num=0;
                    for(int j=0;j<num<<1;j++)
                    {
                        uint32_t rid=(uint32_t)*(row_ptr+pos+j);
						 
                        map<int, int>::iterator iter;
                        iter=_incld_id_map.find(rid);
                        if(iter!=_incld_id_map.end())
                        {
                            int sid=iter->second;							
                            eqtlinfo->_rowid.push_back(sid);
                            eqtlinfo->_val.push_back(*(val_ptr+pos+j));
                            real_num++;
                        }
                    }
                    eqtlinfo->_cols[(i<<1)+1]=(real_num>>1)+eqtlinfo->_cols[i<<1];
                    eqtlinfo->_cols[i+1<<1]=real_num+eqtlinfo->_cols[i<<1];
                }
                eqtlinfo->_valNum = eqtlinfo->_val.size();
                cout<<"eQTL summary-level statistics of "<<eqtlinfo->_include.size()<<" Probes and "<<eqtlinfo->_esi_include.size()<<" SNPs to be included from [" + besdfile + "]." <<endl;
                if(eqtlinfo->_include.size()<eqtlinfo->_probNum ) update_epi(eqtlinfo);
                if(eqtlinfo->_esi_include.size()<eqtlinfo->_snpNum) update_esi(eqtlinfo);
            }
            else
            {
                eqtlinfo->_cols.resize(colNum+1);
                eqtlinfo->_rowid.resize(valNum);
                eqtlinfo->_val.resize(valNum);
                for(int i=0;i<=colNum;i++) eqtlinfo->_cols[i]=(uint32_t)*ptr++;
                for(int i=0;i<valNum;i++) eqtlinfo->_rowid[i]=(uint32_t)*ptr++;
                for(int i=0;i<valNum;i++) eqtlinfo->_val[i]=*ptr++;
                eqtlinfo->_valNum = valNum;
                cout<<"eQTL summary-level statistics of "<<eqtlinfo->_probNum<<" Probes and "<<eqtlinfo->_snpNum<<" SNPs to be included from [" + besdfile + "]." <<endl;
            }
            // terminate
            free (buffer);
        }
        else if ((int)*flag == SPARSE_FILE_TYPE_3)
        {
            // clear datastruct for dense befor read sparse
            eqtlinfo->_bxz.clear();
            eqtlinfo->_sexz.clear();
            
            uint64_t colNum=(eqtlinfo->_probNum<<1)+1;
            uint64_t valNum;
            uint64_t lSize;
            char* buffer;
            besd.seekg(0,besd.end);
            lSize = besd.tellg();
            
            besd.seekg(4); // same as besd.seekg(4, besd.beg);
            besd.read(buf, sizeof(uint64_t));
            valNum=*(uint64_t *)buf;
            if( lSize - (sizeof(float) + sizeof(uint64_t) + colNum*sizeof(uint64_t) + valNum*sizeof(uint32_t) + valNum*sizeof(float)) != 0) {fputs ("wrong element number",stderr); exit (3);}
            
            
            buffer = (char*) malloc (sizeof(char)*(lSize));
            if (buffer == NULL) {fputs ("Memory error",stderr); exit (1);}
            besd.read(buffer,lSize);
            if (besd.gcount()+sizeof(float) + sizeof(uint64_t) != lSize) {fputs ("Reading error",stderr); exit (2);}
            
            
            uint64_t* ptr;
            ptr=(uint64_t *)buffer;
            
            if(eqtlinfo->_include.size()<eqtlinfo->_probNum || eqtlinfo->_esi_include.size()<eqtlinfo->_snpNum)
            {
                eqtlinfo->_cols.resize((eqtlinfo->_include.size()<<1)+1);
                eqtlinfo->_cols[0]=*ptr;
                uint32_t* row_ptr;
                row_ptr=(uint32_t *)(ptr+colNum);
                float* val_ptr;
                val_ptr=(float*)(row_ptr+valNum);
                
                map<int, int > _incld_id_map;
                long size = 0;
                for (int i = 0; i<eqtlinfo->_esi_include.size(); i++)
                {
                    _incld_id_map.insert(pair<int, int>(eqtlinfo->_esi_include[i], i));
                    if (size == _incld_id_map.size()) throw ("Error: Duplicated SNP IDs found: \"" + eqtlinfo->_esi_rs[eqtlinfo->_esi_include[i]] + "\".");
                    size = _incld_id_map.size();
                }
                
                for(int i=0;i<eqtlinfo->_include.size();i++)
                {
                    uint32_t pid=eqtlinfo->_include[i];
                    uint64_t pos=*(ptr+(pid<<1));
                    uint64_t pos1=*(ptr+(pid<<1)+1);
                    uint64_t num=pos1-pos;
                    uint64_t real_num=0;
                    for(int j=0;j<num<<1;j++)
                    {
                        uint32_t rid=*(row_ptr+pos+j);
                        
                        map<int, int>::iterator iter;
                        iter=_incld_id_map.find(rid);
                        if(iter!=_incld_id_map.end())
                        {
                            int sid=iter->second;
                            
                            // long sid=find(eqtlinfo->_esi_include.begin(),eqtlinfo->_esi_include.end(),rid)-eqtlinfo->_esi_include.begin(); //slow
                            //  if(sid<eqtlinfo->_esi_include.size())
                            //  {
                            eqtlinfo->_rowid.push_back(sid);
                            eqtlinfo->_val.push_back(*(val_ptr+pos+j));
                            real_num++;
                        }
                        
                    }
                    eqtlinfo->_cols[(i<<1)+1]=(real_num>>1)+eqtlinfo->_cols[i<<1];
                    eqtlinfo->_cols[i+1<<1]=real_num+eqtlinfo->_cols[i<<1];
                }
                eqtlinfo->_valNum = eqtlinfo->_val.size();
                cout<<"eQTL summary-level statistics of "<<eqtlinfo->_include.size()<<" Probes and "<<eqtlinfo->_esi_include.size()<<" SNPs to be included from [" + besdfile + "]." <<endl;
                if(eqtlinfo->_include.size()<eqtlinfo->_probNum ) update_epi(eqtlinfo);
                if(eqtlinfo->_esi_include.size()<eqtlinfo->_snpNum) update_esi(eqtlinfo);
            }
            else
            {
                eqtlinfo->_cols.resize(colNum);
                eqtlinfo->_rowid.resize(valNum);
                eqtlinfo->_val.resize(valNum);
                
                for(int i=0;i<colNum;i++) eqtlinfo->_cols[i]=*ptr++;
                uint32_t* ptr4B=(uint32_t *)ptr;
                for(int i=0;i<valNum;i++) eqtlinfo->_rowid[i]=*ptr4B++;
                float* val_ptr=(float*)ptr4B;
                for(int i=0;i<valNum;i++) eqtlinfo->_val[i]=*val_ptr++;
                eqtlinfo->_valNum = valNum;
                cout<<"eQTL summary-level statistics of "<<eqtlinfo->_probNum<<" Probes and "<<eqtlinfo->_snpNum<<" SNPs to be included from [" + besdfile + "]." <<endl;
            }
            // terminate
            free (buffer);
        }
        besd.close();
        
        if(eqtlinfo->_rowid.empty() && eqtlinfo->_bxz.empty())
        {
            fputs ("NO data included from eQTL summary data, please check.",stderr); exit (3);
        }
    }
    
    void read_esdfile(eqtlInfo* eqtlinfo, string esdfile)
    {
		// clear datastruct for sparse befor read dense
		eqtlinfo->_cols.clear();
		eqtlinfo->_rowid.clear();
		eqtlinfo->_val.clear();
		eqtlinfo->_valNum = 0;

        ifstream esd;
        if(!file_read_check(&esd, esdfile.c_str()))
        {
            fprintf (stderr, "%s: Couldn't open file %s\n",
                     esdfile.c_str(), strerror (errno));
            exit (EXIT_FAILURE);
        }
        cout << "Reading eQTL summary-level statistics from [" + esdfile + "]." << endl;
        char buf[MAX_LINE_SIZE+4];
        int lineNum(0);      
        while(!esd.eof())
        {
            esd.getline(buf,MAX_LINE_SIZE);
            lineNum++;
        }
        if(buf[0]=='\0') lineNum--;
        if(lineNum != eqtlinfo->_snpNum) throw ("Error: no match of SNP numbers.");
               
		if (eqtlinfo->_include.size()<eqtlinfo->_probNum || eqtlinfo->_esi_include.size()<eqtlinfo->_snpNum)
		{
			eqtlinfo->_bxz.resize(eqtlinfo->_include.size());
			eqtlinfo->_sexz.resize(eqtlinfo->_include.size());
			for (unsigned int i = 0; i<eqtlinfo->_include.size(); i++)
			{
				eqtlinfo->_bxz[i].reserve(eqtlinfo->_esi_include.size());
				eqtlinfo->_sexz[i].reserve(eqtlinfo->_esi_include.size());
			}

			esd.clear(ios::goodbit);
			esd.seekg(0, ios::beg);
			int cur_line = 0;
			for (int i = 0; i<eqtlinfo->_esi_include.size(); i++)
			{
				int sid=eqtlinfo->_esi_include[i];
				for (int j = cur_line; j < sid; j++) esd.getline(buf, MAX_LINE_SIZE);
				string tmpStr;				
				esd.getline(buf, MAX_LINE_SIZE);
				istringstream iss(buf);
				int pos = 0;
				for (int j = 0; j < eqtlinfo->_include.size(); j++)
				{
					for (int k = pos; k < eqtlinfo->_include[j]<<1; k++) iss >> tmpStr;
					iss >> tmpStr;
                    if(!tmpStr.compare("NA")) eqtlinfo->_bxz[j][i]=-9;
                    else eqtlinfo->_bxz[j][i] = atof(tmpStr.c_str());
					iss >> tmpStr;
                    if(!tmpStr.compare("NA")) eqtlinfo->_sexz[j][i] = -9;
                    else eqtlinfo->_sexz[j][i] = atof(tmpStr.c_str());
					pos = (eqtlinfo->_include[j]+1)<<1;
				}
				cur_line = sid + 1;
			}
            if(eqtlinfo->_include.size()<eqtlinfo->_probNum ) update_epi(eqtlinfo);
            if(eqtlinfo->_esi_include.size()<eqtlinfo->_snpNum) update_esi(eqtlinfo);
			/*
			for (int i = 0; i < eqtlinfo->_include.size(); i++)
			{
				for (int j = 0; j < eqtlinfo->_snpNum; j++)
					if (abs(eqtlinfo->_sexz[i][j] + 9) > 1e-6) cout << eqtlinfo->_bxz[i][j] << "," << eqtlinfo->_sexz[i][j] << endl;				
				cout << endl; cout << endl;
			}
			*/			
		}
		else
		{
			eqtlinfo->_bxz.resize(eqtlinfo->_probNum);
			eqtlinfo->_sexz.resize(eqtlinfo->_probNum);
			for (unsigned int i = 0; i<eqtlinfo->_probNum; i++)
			{
				eqtlinfo->_bxz[i].reserve(eqtlinfo->_snpNum);
				eqtlinfo->_sexz[i].reserve(eqtlinfo->_snpNum);
			}

			esd.clear(ios::goodbit);
			esd.seekg(0, ios::beg);
			for (int i = 0; i<lineNum; i++)
			{
				string tmpStr;
				esd.getline(buf, MAX_LINE_SIZE);
				istringstream iss(buf);
				for (int j = 0; j<eqtlinfo->_probNum; j++)
				{
					iss >> tmpStr;
                    if(!tmpStr.compare("NA")) eqtlinfo->_bxz[j][i] = -9;
					else eqtlinfo->_bxz[j][i] = atof(tmpStr.c_str());
					iss >> tmpStr;
                    if(!tmpStr.compare("NA")) eqtlinfo->_sexz[j][i] = -9;
					else eqtlinfo->_sexz[j][i] = atof(tmpStr.c_str());
				}
			}
		}
		     
		cout << "eQTL summary statistics of " << eqtlinfo->_include.size() << " Probes and " << eqtlinfo->_snpNum << " SNPs to be included from [" + esdfile + "]." << endl;
        esd.close();
    }
    void filter_probe_null(eqtlInfo* eqtlinfo)
    {
        vector<string> nullprobes;
        cout<<"\nfiltering out the probes with no value..."<<endl;
        if(eqtlinfo->_valNum==0)
        {
            eqtlinfo->_include.clear();
            for (int i = 0; i < eqtlinfo->_probNum; i++)
            {
                bool NA_flag = true;
                for (int j = 0; j<eqtlinfo->_snpNum; j++)
                {
                    if (abs(eqtlinfo->_sexz[i][j] + 9) > 1e-6)
                    {
                        NA_flag = false;
                        break;
                    }
                }
                if (!NA_flag) eqtlinfo->_include.push_back(i);
                else nullprobes.push_back(eqtlinfo->_epi_prbID[i]);
            }
        }
        else{
            eqtlinfo->_include.clear();
            for (int i = 0; i < eqtlinfo->_probNum; i++)
            {
                if (eqtlinfo->_cols[(i<<1)+1] > eqtlinfo->_cols[i<<1]) eqtlinfo->_include.push_back(i);
                else nullprobes.push_back(eqtlinfo->_epi_prbID[i]);
            }
        }
        if(nullprobes.size()>0)
        {
            string fname="chr"+atos(eqtlinfo->_esi_chr[0])+".nullprobes.log";
            FILE* nullprobefile=fopen(fname.c_str(), "w");
            if (!(nullprobefile)) {
                printf("Error: Failed to open null probe log file.\n");
            }
            for(int i=0;i<nullprobes.size();i++)
            {
                string tmpstr=nullprobes[i]+'\n';
                fputs(tmpstr.c_str(),nullprobefile);
            }
            fclose(nullprobefile);
        }
        
        cout<<eqtlinfo->_include.size()<<" probes to be included."<<endl;
    }

    
    void read_esdfile(eqtlInfo* eqtlinfo, vector<float> &val, vector<uint32_t> &colid, vector<uint32_t> &rowid, string esdfile)
    {
        ifstream esd;
        if(!file_read_check(&esd, esdfile.c_str()))
        {
            fprintf (stderr, "%s: Couldn't open file %s\n",
                     esdfile.c_str(), strerror (errno));
            exit (EXIT_FAILURE);
        }
        cout << "Reading eQTL summary-level statistics from [" + esdfile + "]." << endl;
        char buf[MAX_LINE_SIZE];
        int lineNum(0);
        val.clear();
        colid.clear();
        rowid.clear();
        while(!esd.eof())
        {
            string tmpStr;
            esd.getline(buf, MAX_LINE_SIZE);
            istringstream iss(buf);
            if(buf[0]!='\0')
            {
                for (int j = 0; j<eqtlinfo->_probNum; j++)
                {
                    iss >> tmpStr;
                    if(tmpStr.compare("NA") && tmpStr.compare("-9"))
                    {
                        val.push_back(atof(tmpStr.c_str()));
                        colid.push_back(j<<1);
                        rowid.push_back(lineNum);
                        
                    }
                    iss >> tmpStr;
                    if(tmpStr.compare("NA") && tmpStr.compare("-9"))
                    {
                        val.push_back(atof(tmpStr.c_str()));
                        colid.push_back((j<<1)+1);
                        rowid.push_back(lineNum);
                    }
                }
                lineNum++;
            }
            
        }
        cout << "eQTL summary statistics of " << eqtlinfo->_include.size() << " Probes and " << eqtlinfo->_snpNum << " SNPs to be included from [" + esdfile + "]." << endl;
        esd.close();
    }
    void rmajor2cmajor(eqtlInfo* eqtlinfo, vector<float> &val, vector<uint32_t> &colid, vector<uint32_t> &rowid)
    {
        eqtlinfo->_cols.push_back(0);
        
        for(int i=0;i<eqtlinfo->_probNum<<1;i++)
        {
            uint32_t valnumpercol=0;
            for(int j=0;j<colid.size();j++)
            {
                if(i==colid[j])
                {
                    eqtlinfo->_val.push_back(val[j]);
                    eqtlinfo->_rowid.push_back(rowid[j]);
                    valnumpercol++;
                }
            }
            eqtlinfo->_cols.push_back(eqtlinfo->_cols[i]+valnumpercol);
        }
        eqtlinfo->_valNum=eqtlinfo->_val.size();
        
    }
    void esd2sbesd(char* outFileName, char* eqtlFileName )
    {
         eqtlInfo eqtlinfo;
        cout<<endl<<"Reading eQTL summary data..."<<endl;
        if(eqtlFileName == NULL) throw ("Error: please input the eQTL summary information for the eQTL data files by the option --beqtl-summary.");
        read_esifile(&eqtlinfo, string(eqtlFileName)+".esi");
        read_epifile(&eqtlinfo, string(eqtlFileName)+".epi");
        vector<float> val;
        vector<uint32_t> colid;
        vector<uint32_t> rowid;
        
        read_esdfile(&eqtlinfo, val,colid,rowid, string(eqtlFileName)+".esd");
        
        rmajor2cmajor(&eqtlinfo, val,colid,rowid);
        val.clear();
        colid.clear();
        rowid.clear();
        
        filter_probe_null(&eqtlinfo); // at the same time, reset the vector _include
        cout<<"\nsaving eQTL data..."<<endl;
        string esdfile = string(outFileName)+".esi";
        ofstream smr(esdfile.c_str());
        if (!smr) throw ("Error: can not open the esi file " + esdfile + " to save!");
        for (int i = 0;i <eqtlinfo._snpNum; i++) {
            smr<<eqtlinfo._esi_chr[i]<<'\t'<<eqtlinfo._esi_rs[i]<<'\t'<<eqtlinfo._esi_gd[i]<<'\t'<<eqtlinfo._esi_bp[i]<<'\t'<<eqtlinfo._esi_allele1[i]<<'\t'<<eqtlinfo._esi_allele2[i]<<'\n';
        }
        smr.close();
        cout<<eqtlinfo._snpNum<<" SNPs have been saved in the file [" + esdfile + "]."<<endl;
        
        esdfile = string(outFileName)+".epi";
        smr.open(esdfile.c_str());
        if (!smr) throw ("Error: can not open the epi file " + esdfile + " to save!");
        for (int i = 0;i <eqtlinfo._include.size(); i++) {
            smr<<eqtlinfo._epi_chr[eqtlinfo._include[i]]<<'\t'<<eqtlinfo._epi_prbID[eqtlinfo._include[i]]<<'\t'<<eqtlinfo._epi_gd[eqtlinfo._include[i]]<<'\t'<<eqtlinfo._epi_bp[eqtlinfo._include[i]]<<'\t'<<eqtlinfo._epi_gene[eqtlinfo._include[i]]<<'\t'<<eqtlinfo._epi_orien[eqtlinfo._include[i]]<<'\n';
        }
        smr.close();
        cout<<eqtlinfo._include.size()<<" probes have been saved in the file [" + esdfile + "]."<<endl;
        
        esdfile = string(outFileName)+".besd";
        FILE * smrbesd;
        smrbesd = fopen (esdfile.c_str(), "wb");
           uint64_t colSize=sizeof(uint64_t)*((eqtlinfo._include.size()<<1)+1);
            uint64_t rowSize=sizeof(uint32_t)*eqtlinfo._valNum;
            uint64_t valSize=sizeof(float)*eqtlinfo._valNum;
            uint64_t valNum=eqtlinfo._valNum;
            uint64_t bufsize=sizeof(float)+sizeof(uint64_t)+colSize+rowSize+valSize;
            
            char* buffer=(char*)malloc (sizeof(char)*bufsize);
            memset(buffer,0,sizeof(char)*bufsize);
            float ftype=SPARSE_FILE_TYPE_3;
            memcpy(buffer,&ftype,sizeof(float));
            char* wptr=buffer+sizeof(float);
            memcpy(wptr,&valNum,sizeof(uint64_t));
            wptr+=sizeof(uint64_t);
            uint64_t* uptr=(uint64_t*)wptr; *uptr++=0;
            for(int i=0;i<eqtlinfo._include.size();i++)
            {
                *uptr++=eqtlinfo._cols[(eqtlinfo._include[i]<<1)+1];
                *uptr++=eqtlinfo._cols[eqtlinfo._include[i]+1<<1];
            }
            wptr+=colSize;
            memcpy(wptr,&eqtlinfo._rowid[0],rowSize);
            wptr+=rowSize;
            memcpy(wptr,&eqtlinfo._val[0],valSize);
            fwrite (buffer,sizeof(char), bufsize, smrbesd);
            free(buffer);
       
        fclose (smrbesd);
        
        cout<<"Beta values and SE values for "<<eqtlinfo._include.size()<<" Probes and "<<eqtlinfo._snpNum<<" SNPs have been saved in the binary file [" + esdfile + "]." <<endl;

    }
    
    bool has_suffix(const std::string &str, const std::string &suffix)
    {
        return str.size() >= suffix.size() &&
        str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
    }
    bool has_prefix(const std::string &str, const std::string &prefix)
    {
        return str.size() >= prefix.size() &&
        str.compare(0, prefix.size(), prefix) == 0;
    }

    
  //this function derived from the source code of FastGCN
    void ld_calcualte(double* ref_ld,float* ref_snpData,int rsize,int csize)
    {
        vector<float> tmpX(rsize);
        vector<float> tmpX2(rsize);
        
        //
        //	int Num_CPU_Cores;
        //#ifdef WIN32
        //		SYSTEM_INFO si;
        //		GetSystemInfo(&si);
        //		Num_CPU_Cores=si.dwNumberOfProcessors;
        //#elif
        //		Num_CPU_Cores=sysconf(_SC_NPROCESSORS_CONF);
        //#endif
        //	omp_set_num_threads(Num_CPU_Cores);
        //
        //#pragma omp parallel for
        for(int i=0;i<rsize;i++){
            for(int j=0;j<csize;j++){
                tmpX[i] += ref_snpData[i*csize+j];
                tmpX2[i] += ref_snpData[i*csize+j]*ref_snpData[i*csize+j];
            }
        }
        float tmp0;
        // use lower triangular, because of i>j
        //#pragma omp parallel for private(tmp0)
        for(int i=0;i<rsize;i++){
            for(int j=0;j<i;j++){
                tmp0=0;
                for(int k=0;k<csize;k++) tmp0 += ref_snpData[i*csize+k]*ref_snpData[j*csize+k];
                ref_ld[i*(i-1)/2+j] = 
				(tmp0-tmpX[i]*tmpX[j]/csize)/sqrtf((tmpX2[i]-tmpX[i]*tmpX[i]/csize)*(tmpX2[j]-tmpX[j]*tmpX[j]/csize));
				
            }
        }
        
    }
    
    void get_square_idxes(vector<int> &sn_ids,VectorXd &zsxz,double threshold)
    {
        
        for(int i=0;i<zsxz.size();i++)
        {
           if(zsxz[i]*zsxz[i]-threshold>1e-6) sn_ids.push_back(i);
        }
    }
	void get_square_ldpruning_idxes(vector<int> &sn_ids, VectorXd &zsxz, double threshold, MatrixXd &LD, long maxid, double ld_top)
    {
        for(int i=0;i<zsxz.size();i++)
        {
            if(i!= maxid)
            {
                if((zsxz[i]*zsxz[i]-threshold)>1e-6 && (LD(maxid,i)*LD(maxid,i)-ld_top)<1e-6) sn_ids.push_back(i);
            }
            else{
                 if((zsxz[i]*zsxz[i]-threshold)>1e-6) sn_ids.push_back(i);
            }
            
        }
        
    }
    void get_square_ldpruning_idxes(vector<int> &sn_ids, VectorXd &zsxz, double threshold, VectorXd &ld_v, long maxid, double ld_top)
    {
        for(int i=0;i<zsxz.size();i++)
        {
            if(i!= maxid)
            {
                if((zsxz[i]*zsxz[i]-threshold)>1e-6 && (ld_v(i)*ld_v(i)-ld_top)<1e-6) sn_ids.push_back(i);
            }
            else{
                if((zsxz[i]*zsxz[i]-threshold)>1e-6) sn_ids.push_back(i);
            }
            
        }
        
        
        
    }
   void est_cov_bxy(MatrixXd &covbxy, VectorXd &_zsxz,VectorXd &_bxy,VectorXd &_seyz,VectorXd &_bxz, MatrixXd &_LD_heidi)
    {
        long nsnp =_zsxz.size();
        if(nsnp>1)
        {          
           MatrixXd bxytbxy= _bxy*_bxy.transpose();
           MatrixXd zsxztzsxz= _zsxz*_zsxz.transpose();
           covbxy=_LD_heidi.array()*((_seyz*_seyz.transpose()).array()/(_bxz*_bxz.transpose()).array() + bxytbxy.array()/zsxztzsxz.array()) - bxytbxy.array()/(zsxztzsxz.array()*zsxztzsxz.array());
             
        }
    }

  
   float bxy_hetero3(VectorXd &_byz, VectorXd &_bxz, VectorXd &_seyz, VectorXd &_sexz, VectorXd &_zsxz, MatrixXd &_LD_heidi, long* snp_num)
    {
        VectorXd _bxy;
        VectorXd _sexy;
        VectorXd dev;
        VectorXd tmp3;
        long nsnp=*snp_num;
        int maxid;        
        float pdev=-1.0;
        MatrixXd covbxy(nsnp,nsnp);
        MatrixXd vdev(nsnp-1,nsnp-1);
       
        VectorXd bxz2;
        dev.resize(nsnp-1);
        if(nsnp>1)
        {
            _bxy=_byz.array()/_bxz.array();
            bxz2=_bxz.array()*_bxz.array();
            _sexy=(_seyz.array()*_seyz.array()*bxz2.array()+_sexz.array()*_sexz.array()*_byz.array()*_byz.array())/(bxz2.array()*bxz2.array()).sqrt();
            
            maxid=max_abs_id(_zsxz);
            
            for(int j=0;j<maxid;j++) dev[j]=_bxy[maxid]-_bxy[j];
            for(int j=maxid+1;j<nsnp;j++) dev[j-1]=_bxy[maxid]-_bxy[j];			

            est_cov_bxy(covbxy, _zsxz,_bxy,_seyz,_bxz,_LD_heidi);            
			
            double tmp1=covbxy(maxid,maxid);           
            tmp3.resize(nsnp-1);
            for(int i=0; i<maxid; i++) tmp3[i]=covbxy(maxid,i);
            for(int i=maxid+1; i<nsnp; i++) tmp3[i-1]=covbxy(maxid,i);
            // vdev as tmp2
			vdev.block(0, 0, maxid, maxid) = covbxy.block(0, 0, maxid, maxid);
			vdev.block(0, maxid, maxid, nsnp - maxid - 1) = covbxy.block(0, maxid + 1, maxid, nsnp - maxid - 1);
			vdev.block(maxid, 0, nsnp - maxid - 1, maxid) = covbxy.block(maxid + 1, 0, nsnp - maxid - 1, maxid);
			vdev.block(maxid, maxid, nsnp - maxid - 1, nsnp - maxid - 1) = covbxy.block(maxid + 1, maxid + 1, nsnp - maxid - 1, nsnp - maxid - 1);            
			
			// get vdev
			VectorXd v1 = VectorXd::Zero(nsnp - 1);
			v1 = v1.array() + 1.0;
		
			vdev = tmp1 + vdev.array() - (v1*tmp3.transpose()).array() - (tmp3*v1.transpose()).array();
			for (int i = 0; i<nsnp - 1; i++)  vdev(i,i) += 1e-8; // in R code  		

            //tmp3 as vardev            
            for(int i=0; i<maxid; i++) tmp3[i]=tmp1+covbxy(i,i)-2*tmp3[i]+ 1e-8;
            for(int i=maxid+1; i<nsnp; i++) tmp3[i-1]=tmp1+covbxy(i,i)-2*tmp3[i-1]+ 1e-8;
			
            //dev as chisq_dev
            
            for(int i=0;i<nsnp-1;i++) dev[i]=dev[i]*dev[i]/tmp3[i];
            
            double sumChisq_dev=0.0;
            for(int i=0;i<nsnp-1;i++)sumChisq_dev+=dev[i];
			
            //using covbxy to store corr_dev
			covbxy.resize(nsnp - 1, nsnp - 1);
			covbxy = vdev.array() / sqrt((vdev.diagonal()*vdev.diagonal().transpose()).array());
           
			
            // using Eigen Library
            
			SelfAdjointEigenSolver<MatrixXd> es(covbxy);			
            VectorXd lambda;
            lambda=es.eigenvalues();
            
            /*
             EigenSolver<MatrixXd> es(A);
             cout << "The eigenvalues of A are:" << endl << es.eigenvalues() << endl;
             cout<< es.eigenvalues().transpose()<<endl;
             */
            
            /*
             MatrixXd D = es.pseudoEigenvalueMatrix();
             cout << "The pseudo-eigenvalue matrix D is:" << endl << D << endl;
            */
            
            pdev= pchisqsum(sumChisq_dev,lambda);
            *snp_num=lambda.size();
         
        }else *snp_num=-9;      
        
        return(pdev);
    }
    

    string dtos(double value)
    {
        stringstream ss;
        ss << scientific<< value;
        // ss << fixed << setprecision(400) << __value;
        return(ss.str());
    }
    string dtosf(double value)
    {
        stringstream ss;        
        ss << fixed << value;
        return(ss.str());
    }
    string itos(int value)
    {
        stringstream ss;
        ss << value;      
        return(ss.str());
    }
    string ltos(long value)
    {
        stringstream ss;
        ss << value;
        return(ss.str());
    }

    
    void free_gwas_data(gwasData* gdata)
    {
        gdata->snpName.clear();
        free(gdata->allele_1);
        free(gdata->allele_2);
        free(gdata->freq);
        free(gdata->byz);
        free(gdata->seyz);
        free(gdata->pvalue);
        free(gdata->splSize);
    }
            
    void mu_func(bInfo* bdata, int j, vector<double> &fac) {
        int i = 0;
		bdata->_dosage_flag = 0;
        double fcount = 0.0, f_buf = 0.0;
        if (bdata->_dosage_flag) {
            for (i = 0; i < bdata->_keep.size(); i++) {
                if (bdata->_geno_dose[bdata->_keep[i]][bdata->_include[j]] < 1e5) {
                    bdata->_mu[bdata->_include[j]] += fac[i] * bdata->_geno_dose[bdata->_keep[i]][bdata->_include[j]];
                    fcount += fac[i];
                }
            }
        } else {
            for (i = 0; i < bdata->_keep.size(); i++) {
                if (!bdata->_snp_1[bdata->_include[j]][bdata->_keep[i]] || bdata->_snp_2[bdata->_include[j]][bdata->_keep[i]]) {
                    f_buf = (bdata->_snp_1[bdata->_include[j]][bdata->_keep[i]] + bdata->_snp_2[bdata->_include[j]][bdata->_keep[i]]);
                    if (bdata->_allele2[bdata->_include[j]] == bdata->_ref_A[bdata->_include[j]]) f_buf = 2.0 - f_buf;
                    bdata->_mu[bdata->_include[j]] += fac[i] * f_buf;
                    fcount += fac[i];
                }
            }
        }
        
        if (fcount > 0.0)bdata->_mu[bdata->_include[j]] /= fcount;
    }
    

    void calcu_mu(bInfo* bdata, bool ssq_flag)
    {
        int i = 0, j = 0;
        
        vector<double> auto_fac(bdata->_keep.size()), xfac(bdata->_keep.size()), fac(bdata->_keep.size());
        for (i = 0; i < bdata->_keep.size(); i++)
        {
            auto_fac[i] = 1.0;
            if (bdata->_sex[bdata->_keep[i]] == 1) xfac[i] = 0.5;
            else if (bdata->_sex[bdata->_keep[i]] == 2) xfac[i] = 1.0;
            fac[i] = 0.5;
        }
        
        cout << "Calculating allele frequencies ..." << endl;
        bdata->_mu.clear();
        bdata->_mu.resize(bdata->_snp_num);
        
//#pragma omp parallel for
        for (j = 0; j < bdata->_include.size(); j++)
        {
            if (bdata->_chr[bdata->_include[j]]<(bdata->_autosome_num + 1)) mu_func(bdata, j, auto_fac);
            else if (bdata->_chr[bdata->_include[j]] == (bdata->_autosome_num + 1)) mu_func(bdata,j, xfac);
            else mu_func(bdata, j, fac);
        }
    }

    eigenMatrix reg(vector<double> &y, vector<double> &x, vector<double> &rst, bool table=false)
    {
        int N = x.size();
        if (N != y.size() || N < 1) throw ("Error: The lengths of x and y do not match.");
        
        int i = 0;
        double d_buf = 0.0, y_mu = 0.0, x_mu = 0.0, x_var = 0.0, y_var = 0.0, cov = 0.0;
        for (i = 0; i < N; i++) {
            x_mu += x[i];
            y_mu += y[i];
        }
        x_mu /= (double) N;
        y_mu /= (double) N;
        for (i = 0; i < N; i++) {
            d_buf = (x[i] - x_mu);
            x_var += d_buf*d_buf;
            d_buf = (y[i] - y_mu);
            y_var += d_buf*d_buf;
        }
        x_var /= (double) (N - 1.0);
        y_var /= (double) (N - 1.0);
        for (i = 0; i < N; i++) cov += (x[i] - x_mu)*(y[i] - y_mu);
        cov /= (double) (N - 1);
        double a = 0.0, b = 0.0, sse = 0.0, a_se = 0.0, b_se = 0.0, p = 0.0, rsq = 0.0, r = 0.0;
        if (x_var > 0.0) b = cov / x_var;
        a = y_mu - b*x_mu;
        for (i = 0; i < N; i++) {
            d_buf = y[i] - a - b * x[i];
            sse += d_buf*d_buf;
        }
        if (x_var > 0.0) {
            a_se = sqrt((sse / (N - 2.0))*(1.0 / N + x_mu * x_mu / (x_var * (N - 1.0))));
            b_se = sqrt(sse / x_var / (N - 1.0) / (N - 2.0));
        }
        if (x_var > 0.0 && y_var > 0.0) {
            r = cov / sqrt(y_var * x_var);
            rsq = r*r;
        }
        double t = 0.0;
        if (b_se > 0.0) t = fabs(b / b_se);
        p = StatFunc::t_prob(N - 2.0, t, true);
        rst.clear();
        rst.push_back(b);
        rst.push_back(b_se);
        rst.push_back(p);
        rst.push_back(rsq);
        rst.push_back(r);
        
        eigenMatrix reg_sum(3, 3);
        if (table) {
            reg_sum(2, 0) = rsq;
            reg_sum(1, 0) = b;
            reg_sum(1, 1) = b_se;
            reg_sum(1, 2) = p;
            if (a_se > 0.0) t = fabs(a / a_se);
            p = StatFunc::t_prob(N - 2.0, t, true);
            reg_sum(0, 0) = a;
            reg_sum(0, 1) = a_se;
            reg_sum(0, 2) = p;
            return (reg_sum);
        }
        return (reg_sum);
    }    

    void EstLD(bInfo* bdata, vector<int> &smpl, double wind_size, vector< vector<string> > &snp, vector< vector<double> > &r, vector<double> &r2, vector<double> &md_r2, vector<double> &max_r2, vector<string> &max_r2_snp, vector<double> &dL, vector<double> &dR, vector<int> &K, vector<string> &L_SNP, vector<string> &R_SNP, double alpha, bool IncldQ)
    {
        int i = 0, j = 0, L = 0, R = 0, maxL = 0, maxR = 0, i_buf = 0;
        map<int, int> smpl_snp_map;
        for (i = 0; i < smpl.size(); i++) smpl_snp_map.insert(pair<int, int>(bdata->_include[smpl[i]], i));
        
        cout << "Parameters used to search SNPs in LD with the given SNPs: window size=" << (int) (wind_size * 0.001) << "Kb, significant level=" << alpha << endl;
        vector<double> rst, y, x;
        for (i = 0; i < smpl.size(); i++) {
            vector<int> buf;
            vector<double> r_buf, rsq_buf;
            maxL = maxR = L = R = smpl[i];
            cout<<bdata->_snp_name[bdata->_include[R]]<<endl;
            makex(bdata,L, y);
            if (IncldQ) {
                buf.push_back(L);
                rsq_buf.push_back(1.0);
                r_buf.push_back(1.0);
            }
            while (1) {
                if (R == bdata->_include.size() - 1) break;
                if (bdata->_chr[bdata->_include[R]] != bdata->_chr[bdata->_include[R + 1]]) break;
                if (bdata->_bp[bdata->_include[R + 1]] - bdata->_bp[bdata->_include[smpl[i]]] > wind_size) break;
                R++;
                cout<<bdata->_snp_name[bdata->_include[R]]<<endl;
                if (smpl_snp_map.find(bdata->_include[R]) != smpl_snp_map.end()) continue;
                makex(bdata,R, x);
                reg(y, x, rst);
                if (rst[2] < alpha) {
                    maxR = R;
                    buf.push_back(R);
                    rsq_buf.push_back(rst[3]);
                    r_buf.push_back(rst[4]);
                }
            }
            while (1) {
                if (L == 0) break;
                if (bdata->_chr[bdata->_include[L]] != bdata->_chr[bdata->_include[L - 1]]) break;
                if (bdata->_bp[bdata->_include[smpl[i]]] - bdata->_bp[bdata->_include[L - 1]] > wind_size) break;
                L--;
                cout<<bdata->_snp_name[bdata->_include[L]]<<endl;
                if (smpl_snp_map.find(bdata->_include[L]) != smpl_snp_map.end()) continue;
                makex(bdata, L, x);
                reg(y, x, rst);
                if (rst[2] < alpha) {
                    maxL = L;
                    buf.insert(buf.begin(), L);
                    rsq_buf.insert(rsq_buf.begin(), rst[3]);
                    r_buf.insert(r_buf.begin(), rst[4]);
                }
            }
            if (buf.size() == 0) {
                K[i] = 0;
                dL[i] = 0;
                dR[i] = 0;
                L_SNP[i] = "NA";
                R_SNP[i] = "NA";
                r[i].push_back(0.0);
                snp[i].push_back("NA");
                r2[i] = 0.0;
                md_r2[i] = 0.0;
                max_r2[i] = 0.0;
                max_r2_snp[i] = "NA";
            } else {
                K[i] = buf.size();
                dL[i] = bdata->_bp[bdata->_include[smpl[i]]] - bdata->_bp[bdata->_include[maxL]];
                dR[i] = bdata->_bp[bdata->_include[maxR]] - bdata->_bp[bdata->_include[smpl[i]]];
                L_SNP[i] = bdata->_snp_name[bdata->_include[maxL]];
                R_SNP[i] = bdata->_snp_name[bdata->_include[maxR]];
                for (j = 0; j < K[i]; j++) {
                    r[i].push_back(r_buf[j]);
                    snp[i].push_back(bdata->_snp_name[bdata->_include[buf[j]]]);
                }
                r2[i] = CommFunc::mean(rsq_buf);
                md_r2[i] = CommFunc::median(rsq_buf);
                i_buf = max_element(rsq_buf.begin(), rsq_buf.end()) - rsq_buf.begin();
                max_r2[i] = rsq_buf[i_buf];
                max_r2_snp[i] = snp[i][i_buf];
            }
            cout << i + 1 << " of " << smpl.size() << " target SNPs.\r";
        }
    }
    // est LD values for one SNP versus the SNPs around it.  
    void LD_Blocks(bInfo* bdata, double wind_size, double alpha, bool IncldQ, vector<string> &_ld_target_snp)
    {
        int i = 0, j = 0;
        
        // Read snplist file
        vector<int> smpl_buf, smpl;
        vector<string> uni_snp;
        for (i = 0; i < bdata->_include.size(); i++) uni_snp.push_back(bdata->_snp_name[bdata->_include[i]]);
        StrFunc::match(_ld_target_snp, uni_snp, smpl_buf);
        for (i = 0; i < smpl_buf.size(); i++) {
            if (smpl_buf[i]>-1) smpl.push_back(smpl_buf[i]);
        }
        int SNP_SmplNum = smpl.size(); // smpl is the position of _include        
        // Calculate LD structure
        cout << "Estimating LD structure..." << endl;
        vector<int> K(SNP_SmplNum);
        vector<double> r2(SNP_SmplNum), md_r2(SNP_SmplNum), max_r2(SNP_SmplNum), dL(SNP_SmplNum), dR(SNP_SmplNum);
        vector<string> max_r2_snp(SNP_SmplNum);
        vector< vector<double> > r(SNP_SmplNum);
        vector<string> L_SNP(SNP_SmplNum), R_SNP(SNP_SmplNum);
        vector< vector<string> > snp_ls(SNP_SmplNum);
        
        
        EstLD(bdata, smpl, wind_size, snp_ls, r, r2, md_r2, max_r2, max_r2_snp, dL, dR, K, L_SNP, R_SNP, alpha, IncldQ);        
        
        
       
        cout << "target_SNP\tfreq\tL_region\tR_region\tL_snp\tR_snp\tnSNPs\tmean_rsq\tmedian_rsq\tmax_rsq\tmax_rsq_snp" << endl;
        for (i = 0; i < SNP_SmplNum; i++) cout << bdata->_snp_name[bdata->_include[smpl[i]]] << "\t" << "\t" << dL[i] << "\t" << dR[i] << "\t" << L_SNP[i] << "\t" << R_SNP[i] << "\t" << K[i] << "\t" << r2[i] << "\t" << md_r2[i] << "\t" << max_r2[i] << "\t" << max_r2_snp[i] << endl;
       
        
        for (i = 0; i < SNP_SmplNum; i++) {
            for (j = 0; j < r[i].size(); j++) cout << r[i][j] << " ";
            cout << endl;
        }
        
        for (i = 0; i < SNP_SmplNum; i++) {
            for (j = 0; j < snp_ls[i].size(); j++) cout << snp_ls[i][j] << " ";
            cout << endl;
        }       
    }
    
    bool make_XMat(bInfo* bdata, MatrixXf &X)
    {
        if (bdata->_mu.empty()) calcu_mu(bdata);
        
        cout << "Recoding genotypes (individual major mode) ..." << endl;
        bool have_mis = false;
        unsigned long i = 0, j = 0, n = bdata->_keep.size(), m = bdata->_include.size();
        
        X.resize(0,0);
        X.resize(n, m);
//#pragma omp parallel for private(j)
        for (i = 0; i < n; i++) {
            if (bdata->_dosage_flag) {
                for (j = 0; j < m; j++) {
                    if (bdata->_geno_dose[bdata->_keep[i]][bdata->_include[j]] < 1e5) {
                        if (bdata->_allele1[bdata->_include[j]] == bdata->_ref_A[bdata->_include[j]]) X(i,j) = bdata->_geno_dose[bdata->_keep[i]][bdata->_include[j]];
                        else X(i,j) = 2.0 - bdata->_geno_dose[bdata->_keep[i]][bdata->_include[j]];
                    }
                    else {
                        X(i,j) = 1e6;
                        have_mis = true;
                    }
                }
                bdata->_geno_dose[i].clear();
            }
            else {
                for (j = 0; j < bdata->_include.size(); j++) {
                    if (!bdata->_snp_1[bdata->_include[j]][bdata->_keep[i]] || bdata->_snp_2[bdata->_include[j]][bdata->_keep[i]]) {
                        if (bdata->_allele1[bdata->_include[j]] == bdata->_ref_A[bdata->_include[j]]) X(i,j) = bdata->_snp_1[bdata->_include[j]][bdata->_keep[i]] + bdata->_snp_2[bdata->_include[j]][bdata->_keep[i]];
                        else X(i,j) = 2.0 - (bdata->_snp_1[bdata->_include[j]][bdata->_keep[i]] + bdata->_snp_2[bdata->_include[j]][bdata->_keep[i]]);
                    }
                    else {
                        X(i,j) = 1e6;
                        have_mis = true;
                    }
                }
            }
        }
        return have_mis;
    }
   
    // SNP mode: each vector is the genotype of a SNP with all of individuals
    void make_XMat_SNPs(bInfo* bdata, vector< vector<float> > &X, bool miss_with_mu)
    {
        if(bdata->_mu.empty() && miss_with_mu) calcu_mu(bdata);
        
        cout<<"Recoding genotypes (SNP major mode) ..."<<endl;
        int i=0, j=0;
        X.clear();
        X.resize(bdata->_include.size());
        for(i=0; i<bdata->_include.size(); i++) X[i].resize(bdata->_keep.size());
        for(i=0; i<bdata->_keep.size(); i++){
            bool need2fill=false;
            if(bdata->_dosage_flag){
                for(j=0; j<bdata->_include.size(); j++){
                    if(bdata->_geno_dose[bdata->_keep[i]][bdata->_include[j]]<1e5){
                        if(bdata->_allele1[bdata->_include[j]]==bdata->_ref_A[bdata->_include[j]]) X[j][i]=bdata->_geno_dose[bdata->_keep[i]][bdata->_include[j]];
                        else X[j][i]=2.0-bdata->_geno_dose[bdata->_keep[i]][bdata->_include[j]];
                    }
                    else{ X[j][i]=1e6; need2fill=true; }
                }
                bdata->_geno_dose[i].clear();
            }
            else{
                for(j=0; j<bdata->_include.size(); j++){
                    if(!bdata->_snp_1[bdata->_include[j]][bdata->_keep[i]] || bdata->_snp_2[bdata->_include[j]][bdata->_keep[i]]){
                        if(bdata->_allele1[bdata->_include[j]]==bdata->_ref_A[bdata->_include[j]]) X[j][i]=(bdata->_snp_1[bdata->_include[j]][bdata->_keep[i]]+bdata->_snp_2[bdata->_include[j]][bdata->_keep[i]]);
                        else X[j][i]=2.0-(bdata->_snp_1[bdata->_include[j]][bdata->_keep[i]]+bdata->_snp_2[bdata->_include[j]][bdata->_keep[i]]);
                    }
                    else{ X[j][i]=1e6; need2fill=true; }
                }
            }
            // Fill the missing genotype with the mean of x (2p)
            for(j=0; j<bdata->_include.size() && miss_with_mu && need2fill; j++){
                if(X[j][i]>1e5) X[j][i]=bdata->_mu[bdata->_include[j]];
            }
        }
    }
    
    void makex_eigenVector(bInfo* bdata,int j, eigenVector &x, bool resize, bool minus_2p)
    {
        int i = 0;
        if (resize) x.resize(bdata->_keep.size());
        for (i = 0; i < bdata->_keep.size(); i++) {
            if (!bdata->_snp_1[bdata->_include[j]][bdata->_keep[i]] || bdata->_snp_2[bdata->_include[j]][bdata->_keep[i]]) {
                if (bdata->_allele1[bdata->_include[j]] == bdata->_ref_A[bdata->_include[j]]) x[i] = (bdata->_snp_1[bdata->_include[j]][bdata->_keep[i]] + bdata->_snp_2[bdata->_include[j]][bdata->_keep[i]]);
                else x[i] = 2.0 - (bdata->_snp_1[bdata->_include[j]][bdata->_keep[i]] + bdata->_snp_2[bdata->_include[j]][bdata->_keep[i]]);
            } else x[i] = bdata->_mu[bdata->_include[j]];
            if (minus_2p) x[i] -= bdata->_mu[bdata->_include[j]];
        }
    }
    
       
    void makeptrx(bInfo* bdata,int bsnpid,int cursnpid, float* X, bool minus_2p)
    {
        int i = 0;
        for (i = 0; i < bdata->_keep.size(); i++) {
            if (!bdata->_snp_1[bdata->_include[bsnpid]][bdata->_keep[i]] || bdata->_snp_2[bdata->_include[bsnpid]][bdata->_keep[i]]) {
                if (bdata->_allele1[bdata->_include[bsnpid]] == bdata->_ref_A[bdata->_include[bsnpid]]) X[cursnpid*bdata->_indi_num+i] = (bdata->_snp_1[bdata->_include[bsnpid]][bdata->_keep[i]] + bdata->_snp_2[bdata->_include[bsnpid]][bdata->_keep[i]]);
                else X[cursnpid*bdata->_indi_num+i] = 2.0 - (bdata->_snp_1[bdata->_include[bsnpid]][bdata->_keep[i]] + bdata->_snp_2[bdata->_include[bsnpid]][bdata->_keep[i]]);
            } else X[cursnpid*bdata->_indi_num+i] = bdata->_mu[bdata->_include[bsnpid]];
            if (minus_2p) X[cursnpid*bdata->_indi_num+i] -= bdata->_mu[bdata->_include[bsnpid]];
        }
    }
    
    void read_indi_list(string indi_list_file, vector<string> &indi_list)
    {
        ifstream i_indi_list(indi_list_file.c_str());
        if(!i_indi_list) throw("Error: can not open the file ["+indi_list_file+"] to read.");
        string str_buf, id_buf;
        indi_list.clear();
        while(i_indi_list){
            i_indi_list>>str_buf;
            if(i_indi_list.eof()) break;
            id_buf=str_buf+":";
            i_indi_list>>str_buf;
            id_buf+=str_buf;
            indi_list.push_back(id_buf);
            getline(i_indi_list, str_buf);
        }
        i_indi_list.close();
    }
    
    void read_msglist(string msglistfile, vector<string> &msglist, string msg)
    {
        // Read msglist file
        msglist.clear();
        string StrBuf;
        ifstream i_msglist(msglistfile.c_str());
        if(!i_msglist) throw("Error: can not open the file ["+msglistfile+"] to read.");
        cout<<"Reading a list of "<<msg<<" from ["+msglistfile+"]."<<endl;
        while(i_msglist>>StrBuf){
            msglist.push_back(StrBuf);
            getline(i_msglist, StrBuf);
        }
        i_msglist.close();
    }
    
    
    void update_id_map_kp(const vector<string> &id_list, map<string, int> &id_map, vector<int> &keep)
    {
        int i=0;
        map<string, int> id_map_buf(id_map);
        for(i=0; i<id_list.size(); i++) id_map_buf.erase(id_list[i]);
        map<string, int>::iterator iter;
        for(iter=id_map_buf.begin(); iter!=id_map_buf.end(); iter++) id_map.erase(iter->first);
        
        keep.clear();
        for(iter=id_map.begin(); iter!=id_map.end(); iter++) keep.push_back(iter->second);
        stable_sort(keep.begin(), keep.end());
    }
    void update_id_map_rm(const vector<string> &id_list, map<string, int> &id_map, vector<int> &keep)
    {
        int i = 0;
        for (i = 0; i < id_list.size(); i++) id_map.erase(id_list[i]);
        
        keep.clear();
        map<string, int>::iterator iter;
        for (iter = id_map.begin(); iter != id_map.end(); iter++) keep.push_back(iter->second);
        stable_sort(keep.begin(), keep.end());
    }
    

    void keep_indi(bInfo* bdata,string indi_list_file)
    {
        vector<string> indi_list;
        read_indi_list(indi_list_file, indi_list);
        update_id_map_kp(indi_list, bdata->_id_map, bdata->_keep);
        cout<<bdata->_keep.size()<<" individuals are kept from ["+indi_list_file+"]."<<endl;
    }
    
    void remove_indi(bInfo* bdata, string indi_list_file) {
        vector<string> indi_list;
        read_indi_list(indi_list_file, indi_list);
        int prev_size = bdata->_keep.size();
        update_id_map_rm(indi_list, bdata->_id_map, bdata->_keep);
        cout << prev_size - bdata->_keep.size() << " individuals are removed from [" + indi_list_file + "] and there are " << bdata->_keep.size() << " individuals remaining." << endl;
    }

    void extract_snp(bInfo* bdata,string snplistfile)
    {
        vector<string> snplist;
        string msg="SNPs";
        read_msglist(snplistfile, snplist,msg);
        update_id_map_kp(snplist, bdata->_snp_name_map, bdata->_include);
        cout<<bdata->_include.size()<<" SNPs are extracted from ["+snplistfile+"]."<<endl;
    }
    void exclude_snp(bInfo* bdata,string snplistfile)
    {
        vector<string> snplist;
         string msg="SNPs";
        read_msglist(snplistfile, snplist,msg);
        int prev_size = bdata->_include.size();
        update_id_map_rm(snplist, bdata->_snp_name_map, bdata->_include);
        cout << prev_size - bdata->_include.size() << " SNPs are excluded from [" + snplistfile + "] and there are " << bdata->_include.size() << " SNPs remaining." << endl;
    }
   
	void extract_eqtl_snp(eqtlInfo* eqtlinfo, string snplstName)
	{
		vector<string> snplist;
		string msg = "SNPs";
		read_msglist(snplstName, snplist, msg);
		eqtlinfo->_esi_include.clear();
		StrFunc::match_only(snplist, eqtlinfo->_esi_rs, eqtlinfo->_esi_include);
		stable_sort(eqtlinfo->_esi_include.begin(), eqtlinfo->_esi_include.end());        
		cout << eqtlinfo->_esi_include.size() << " SNPs are extracted from [" + snplstName + "]." << endl;
	}
    
    void exclude_eqtl_snp(eqtlInfo* eqtlinfo, string snplstName)
    {
        vector<string> snplist;
        vector<string> mapstr;
         vector<int> tmp;
        string msg = "SNPs";
        read_msglist(snplstName, snplist, msg);
        int pre_num=eqtlinfo->_esi_include.size();
        mapstr.resize(pre_num);
        tmp.resize(pre_num);
        for(int i=0;i<pre_num;i++)
        {
            mapstr[i]=eqtlinfo->_esi_rs[eqtlinfo->_esi_include[i]];
            tmp[i]=eqtlinfo->_esi_include[i];
        }
        
        eqtlinfo->_esi_include.clear();
        StrFunc::set_complement(snplist, mapstr, tmp, eqtlinfo->_esi_include); //sorted
        stable_sort(eqtlinfo->_esi_include.begin(), eqtlinfo->_esi_include.end());       
        cout << pre_num-eqtlinfo->_esi_include.size() << " SNPs are excluded from [" + snplstName + "] and there are " << eqtlinfo->_esi_include.size() << " SNPs remaining." << endl;
    }
    
    void extract_gwas_snp(gwasData* gdata, string snplstName)
    {
        vector<string> snplist;
        string msg = "SNPs";
        read_msglist(snplstName, snplist, msg);
        gdata->_include.clear();
        StrFunc::match_only(snplist, gdata->snpName, gdata->_include);
        stable_sort(gdata->_include.begin(), gdata->_include.end());
        cout << gdata->_include.size() << " SNPs are extracted from [" + snplstName + "]." << endl;
    }
    
    void exclude_gwas_snp(gwasData* gdata, string snplstName)
    {
        vector<string> snplist;
        vector<string> mapstr;
         vector<int> tmp;
        string msg = "SNPs";
        read_msglist(snplstName, snplist, msg);
        int pre_num=gdata->_include.size();
        mapstr.resize(pre_num);
         tmp.resize(pre_num);
        for(int i=0;i<pre_num;i++)
        {
            mapstr[i]=gdata->snpName[gdata->_include[i]];
            tmp[i]=gdata->_include[i];
        }
        
        gdata->_include.clear();
        StrFunc::set_complement(snplist, mapstr, tmp,gdata->_include); //sorted
        stable_sort(gdata->_include.begin(), gdata->_include.end());
        cout << pre_num-gdata->_include.size() << " SNPs are excluded from [" + snplstName + "] and there are " << gdata->_include.size() << " SNPs remaining." << endl;
    }

    
    void extract_prob(eqtlInfo* eqtlinfo,string problstName)
    {
        vector<string> problist;
        string msg="probes";
        read_msglist(problstName, problist,msg);
        vector<string> raw_problist;
        for(int i=0;i<eqtlinfo->_include.size();i++) raw_problist.push_back(eqtlinfo->_epi_prbID[eqtlinfo->_include[i]]);
        vector<string> common_probes;
        set_intersect(problist, raw_problist, common_probes);
        eqtlinfo->_include.clear();
		StrFunc::match_only(common_probes, eqtlinfo->_epi_prbID, eqtlinfo->_include);
        stable_sort(eqtlinfo->_include.begin(), eqtlinfo->_include.end());
        cout<<eqtlinfo->_include.size()<<" probes are extracted from ["+problstName+"]."<<endl;
    }
    
    void extract_prob_by_gene(eqtlInfo* eqtlinfo, string genelistName)
    {
        vector<string> genelist;
        string msg="genes";
        read_msglist(genelistName, genelist,msg);
        vector<string> raw_problist;
        vector<string> raw_genelist;
        for(int i=0;i<eqtlinfo->_include.size();i++)
        {
            raw_problist.push_back(eqtlinfo->_epi_prbID[eqtlinfo->_include[i]]);
            raw_genelist.push_back(eqtlinfo->_epi_gene[eqtlinfo->_include[i]]);
        }
        vector<int> idx;
        for(int i=0;i<genelist.size();i++)
        {
            string tmpname1=genelist[i];
            for(int j=0;j<raw_genelist.size();j++)
            {
                string tmpname2=raw_genelist[j];
                if(tmpname1==tmpname2)  idx.push_back(j);
                else
                {
                    vector<string> substrs;
                    int tmpnum=split_string(tmpname2,substrs);
                    if(tmpnum>1)
                        for(int k=0;k<tmpnum;k++)
                            if(tmpname1==substrs[k])
                            {
                                idx.push_back(j);
                                break;
                            }
                }
            }
            out2:;
        }
        vector<string> common_probes;
        for(int i=0;i<idx.size();i++) common_probes.push_back(raw_problist[idx[i]]);
        eqtlinfo->_include.clear();
        StrFunc::match_only(common_probes, eqtlinfo->_epi_prbID, eqtlinfo->_include);
        stable_sort(eqtlinfo->_include.begin(), eqtlinfo->_include.end());
        cout<<eqtlinfo->_include.size()<<" probes are extracted from ["+genelistName+"]."<<endl;
    }
    void exclude_prob(eqtlInfo* eqtlinfo,string problstName)
    {
        vector<string> problist;
        vector<string> mappro;
        vector<int> tmp;
        string msg="probes";
        read_msglist(problstName, problist,msg);
        int pre_num=eqtlinfo->_include.size();
        mappro.resize(pre_num);
        tmp.resize(pre_num);
        for(int i=0;i<pre_num;i++)
        {
            mappro[i]=eqtlinfo->_epi_prbID[eqtlinfo->_include[i]];
            tmp[i]=eqtlinfo->_include[i];
        }
       
        eqtlinfo->_include.clear();
        StrFunc::set_complement(problist, mappro, tmp, eqtlinfo->_include);
        
        cout<<pre_num-eqtlinfo->_include.size()<<" probes are excluded from ["+problstName+"]and there are "<<eqtlinfo->_include.size()<<" probes remaining."<<endl;
    }

    void filter_snp_maf(bInfo* bdata,double maf)
    {
        if(bdata->_mu.empty()) calcu_mu(bdata);
        
        cout<<"Pruning SNPs with MAF > "<<maf<<" ..."<<endl;
        map<string, int> id_map_buf(bdata->_snp_name_map);
        map<string, int>::iterator iter, end=id_map_buf.end();
        int prev_size=bdata->_include.size();
        double fbuf=0.0;
        bdata->_include.clear();
        bdata->_snp_name_map.clear();
        for(iter=id_map_buf.begin(); iter!=end; iter++){
            fbuf=bdata->_mu[iter->second]*0.5;
            if(fbuf<=maf || (1.0-fbuf)<=maf) continue;
            bdata->_snp_name_map.insert(*iter);
            bdata->_include.push_back(iter->second);
        }
        if(bdata->_include.size()==0) throw("Error: No SNP is retained for analysis.");
        else{
            stable_sort(bdata->_include.begin(), bdata->_include.end());
            cout<<"After pruning SNPs with MAF > "<<maf<<", there are "<<bdata->_include.size()<<" SNPs ("<<prev_size-bdata->_include.size()<<" SNPs with MAF < "<<maf<<")."<<endl;
        }
    }
    
    void allele_check(bInfo* bdata, gwasData* gdata, eqtlInfo* esdata)
    {
        // get the common SNPs
        vector<string> slctSNPs;
        vector<string> b_snp_name;
        vector<string> cmmnSNPs;
        vector<int> bdId;
        vector<int> gdId;
        vector<int> edId;
        cmmnSNPs.clear();
        edId.clear();
        string logstr="Performing allele check between GWAS summary dataset, eQTL summary dataset and Plink dataset. ";
        cout<<logstr<<endl;
        
        vector<string> bsnp;
        vector<string> essnp;
        if(bdata->_include.size()< bdata->_snp_num || esdata->_esi_include.size()<esdata->_snpNum )
        {
            for(int i=0;i<bdata->_include.size();i++) bsnp.push_back(bdata->_snp_name[bdata->_include[i]]);
            for(int i=0;i<esdata->_esi_include.size();i++) essnp.push_back(esdata->_esi_rs[esdata->_esi_include[i]]);
            StrFunc::match_only(bsnp, gdata->snpName, edId);
            if(edId.empty()) throw("Error: no common SNPs between PLink data and GWAS data.");
            for(int i=0;i<edId.size();i++) cmmnSNPs.push_back(gdata->snpName[edId[i]]);
            StrFunc::match_only(cmmnSNPs, essnp, edId);
            if(edId.empty()) throw("Error: no common SNPs found.");
            for(int i=0;i<edId.size();i++) edId[i]=esdata->_esi_include[edId[i]];
            for(int i=0;i<edId.size();i++) slctSNPs.push_back(esdata->_esi_rs[edId[i]]);
        }else
        {
            StrFunc::match_only(bdata->_snp_name, gdata->snpName, edId);
            if(edId.empty()) throw("Error: no common SNPs between PLink data and GWAS data.");
            for(int i=0;i<edId.size();i++) cmmnSNPs.push_back(gdata->snpName[edId[i]]);
            edId.clear();
            StrFunc::match_only(cmmnSNPs, esdata->_esi_rs, edId);
            if(edId.empty()) throw("Error: no common SNPs found.");
            for(int i=0;i<edId.size();i++) slctSNPs.push_back(esdata->_esi_rs[edId[i]]);
        }
        
        //slctSNPs is in the order as bdata. so bdId is in increase order. edId and gdId may not.
        
        //alleles check
        StrFunc::match(slctSNPs, bdata->_snp_name, bdId);
        StrFunc::match(slctSNPs, gdata->snpName, gdId);
        cmmnSNPs.clear();
        bdata->_include.clear();
        gdata->_include.clear();
        esdata->_esi_include.clear();
        
        for (int i = 0; i<edId.size(); i++)
        {
            char a1, a2, ga1, ga2, ea1, ea2;
            a1 = bdata->_allele1[bdId[i]];
            a2 = bdata->_allele2[bdId[i]];
            ga1 = gdata->allele_1[gdId[i]];
            ga2 = gdata->allele_2[gdId[i]];
            ea1 = esdata->_esi_allele1[edId[i]];
            ea2 = esdata->_esi_allele2[edId[i]];
            // use the allele in eQTL summary data as the reference allele. so we won't get the whole besd into memroy
            if(ea1 == a1 &&  ea2 == a2)
            {
                if( ea1 == ga1 && ea2 == ga2)
                {
                    
                    bdata->_include.push_back(bdId[i]);
                    gdata->_include.push_back(gdId[i]);
                    esdata->_esi_include.push_back(edId[i]);
                }
                else if(ea1 == ga2 && ea2 == ga1)
                {
                    bdata->_include.push_back(bdId[i]);
                    gdata->_include.push_back(gdId[i]);
                    esdata->_esi_include.push_back(edId[i]);
                    
                    gdata->byz[gdId[i]]=-gdata->byz[gdId[i]];
                    
                }
            }
            else if(ea1 == a2 &&  ea2 == a1)
            {
                
                if( ea1 == ga1 && ea2 == ga2)
                {
                    bdata->_include.push_back(bdId[i]);
                    gdata->_include.push_back(gdId[i]);
                    esdata->_esi_include.push_back(edId[i]);
                    
                    char tmpch=bdata->_ref_A[bdId[i]];
                    bdata->_ref_A[bdId[i]]=bdata->_other_A[bdId[i]];
                    bdata->_other_A[bdId[i]]=tmpch;
                   
                }
                else if(ea1 == ga2 && ea2 == ga1)
                {
                    bdata->_include.push_back(bdId[i]);
                    gdata->_include.push_back(gdId[i]);
                    esdata->_esi_include.push_back(edId[i]);
                    
                    gdata->byz[gdId[i]]=-gdata->byz[gdId[i]];
                    char tmpch=bdata->_ref_A[bdId[i]];
                    bdata->_ref_A[bdId[i]]=bdata->_other_A[bdId[i]];
                    bdata->_other_A[bdId[i]]=tmpch;
                    
                }
            }

            
        }
        
        logstr=itos(bdata->_include.size())+" SNPs are included after allele check. ";
        cout<<logstr<<endl;
        
        // only update _snp_name_map which would be used in read bed file.
        map<string, int> id_map_buf(bdata->_snp_name_map);
        for(int i=0; i<bdata->_include.size(); i++) id_map_buf.erase(bdata->_snp_name[bdata->_include[i]]);
        map<string, int>::iterator iter;
        for(iter=id_map_buf.begin(); iter!=id_map_buf.end(); iter++) bdata->_snp_name_map.erase(iter->first);
    }
    
    bool allele_check_(bInfo* bdata, gwasData* gdata, eqtlInfo* esdata)
    {
        // get the common SNPs
        vector<string> slctSNPs;
        vector<string> b_snp_name;
        vector<string> cmmnSNPs;
        vector<int> bdId;
        vector<int> gdId;
        vector<int> edId;
        cmmnSNPs.clear();
        edId.clear();
        string logstr="Performing allele check between GWAS summary dataset, eQTL summary dataset and Plink dataset. ";
        cout<<logstr<<endl;
        
        vector<string> bsnp;
        vector<string> essnp;
        if(bdata->_include.size()< bdata->_snp_num || esdata->_esi_include.size()<esdata->_snpNum )
        {
            for(int i=0;i<bdata->_include.size();i++) bsnp.push_back(bdata->_snp_name[bdata->_include[i]]);
            for(int i=0;i<esdata->_esi_include.size();i++) essnp.push_back(esdata->_esi_rs[esdata->_esi_include[i]]);
            StrFunc::match_only(bsnp, gdata->snpName, edId);
            if(edId.empty()) return false;
            for(int i=0;i<edId.size();i++) cmmnSNPs.push_back(gdata->snpName[edId[i]]);
            StrFunc::match_only(cmmnSNPs, essnp, edId);
            if(edId.empty()) return false;
            for(int i=0;i<edId.size();i++) edId[i]=esdata->_esi_include[edId[i]];
            for(int i=0;i<edId.size();i++) slctSNPs.push_back(esdata->_esi_rs[edId[i]]);
        }else
        {
            StrFunc::match_only(bdata->_snp_name, gdata->snpName, edId);
            if(edId.empty()) return false;
            for(int i=0;i<edId.size();i++) cmmnSNPs.push_back(gdata->snpName[edId[i]]);
            edId.clear();
            StrFunc::match_only(cmmnSNPs, esdata->_esi_rs, edId);
            if(edId.empty()) return false;
            for(int i=0;i<edId.size();i++) slctSNPs.push_back(esdata->_esi_rs[edId[i]]);
        }
        
        //slctSNPs is in the order as bdata. so bdId is in increase order. edId and gdId may not.
        
        //alleles check
        StrFunc::match(slctSNPs, bdata->_snp_name, bdId);
        StrFunc::match(slctSNPs, gdata->snpName, gdId);
        cmmnSNPs.clear();
        bdata->_include.clear();
        gdata->_include.clear();
        esdata->_esi_include.clear();
        
        for (int i = 0; i<edId.size(); i++)
        {
            char a1, a2, ga1, ga2, ea1, ea2;
            a1 = bdata->_allele1[bdId[i]];
            a2 = bdata->_allele2[bdId[i]];
            ga1 = gdata->allele_1[gdId[i]];
            ga2 = gdata->allele_2[gdId[i]];
            ea1 = esdata->_esi_allele1[edId[i]];
            ea2 = esdata->_esi_allele2[edId[i]];
            // use the allele in eQTL summary data as the reference allele. so we won't get the whole besd into memroy
            if(ea1 == a1 &&  ea2 == a2)
            {
                if( ea1 == ga1 && ea2 == ga2)
                {
                    
                    bdata->_include.push_back(bdId[i]);
                    gdata->_include.push_back(gdId[i]);
                    esdata->_esi_include.push_back(edId[i]);
                }
                else if(ea1 == ga2 && ea2 == ga1)
                {
                    bdata->_include.push_back(bdId[i]);
                    gdata->_include.push_back(gdId[i]);
                    esdata->_esi_include.push_back(edId[i]);
                    
                    gdata->byz[gdId[i]]=-gdata->byz[gdId[i]];
                    
                }
            }
            else if(ea1 == a2 &&  ea2 == a1)
            {
                
                if( ea1 == ga1 && ea2 == ga2)
                {
                    bdata->_include.push_back(bdId[i]);
                    gdata->_include.push_back(gdId[i]);
                    esdata->_esi_include.push_back(edId[i]);
                    
                    char tmpch=bdata->_ref_A[bdId[i]];
                    bdata->_ref_A[bdId[i]]=bdata->_other_A[bdId[i]];
                    bdata->_other_A[bdId[i]]=tmpch;
                    
                }
                else if(ea1 == ga2 && ea2 == ga1)
                {
                    bdata->_include.push_back(bdId[i]);
                    gdata->_include.push_back(gdId[i]);
                    esdata->_esi_include.push_back(edId[i]);
                    
                    gdata->byz[gdId[i]]=-gdata->byz[gdId[i]];
                    char tmpch=bdata->_ref_A[bdId[i]];
                    bdata->_ref_A[bdId[i]]=bdata->_other_A[bdId[i]];
                    bdata->_other_A[bdId[i]]=tmpch;
                    
                }
            }
            
            
        }
        
        logstr=itos(bdata->_include.size())+" SNPs are included after allele check. ";
        cout<<logstr<<endl;
        
        // only update _snp_name_map which would be used in read bed file.
        map<string, int> id_map_buf(bdata->_snp_name_map);
        for(int i=0; i<bdata->_include.size(); i++) id_map_buf.erase(bdata->_snp_name[bdata->_include[i]]);
        map<string, int>::iterator iter;
        for(iter=id_map_buf.begin(); iter!=id_map_buf.end(); iter++) bdata->_snp_name_map.erase(iter->first);
        
        return true;
        
    }
    
    void allele_check(gwasData* gdata, eqtlInfo* esdata)
    {
        // get the common SNPs
        vector<string> slctSNPs;
        vector<string> cmmnSNPs;
        vector<int> gdId;
        vector<int> edId;
        cmmnSNPs.clear();
        edId.clear();
        string logstr="Performing allele check between GWAS summary dataset and eQTL summary dataset. ";
        cout<<logstr<<endl;
       
        vector<string> essnp;
        if(esdata->_esi_include.size()<esdata->_snpNum )
        {
            for(int i=0;i<esdata->_esi_include.size();i++) essnp.push_back(esdata->_esi_rs[esdata->_esi_include[i]]);
            StrFunc::match_only(essnp, gdata->snpName, gdId);
            if(gdId.empty()) throw("Error: no common SNPs found.");
            for(int i=0;i<gdId.size();i++) slctSNPs.push_back(gdata->snpName[gdId[i]]);
        }else
        {
            StrFunc::match_only(esdata->_esi_rs, gdata->snpName, gdId);
            if(gdId.empty()) throw("Error: no common SNPs found.");
            for(int i=0;i<gdId.size();i++) slctSNPs.push_back(gdata->snpName[gdId[i]]);
        }
        
        
        //alleles check
        StrFunc::match(slctSNPs, esdata->_esi_rs, edId);
        cmmnSNPs.clear();
        gdata->_include.clear();
        esdata->_esi_include.clear();
        
        for (int i = 0; i<edId.size(); i++)
        {
            char ga1, ga2, ea1, ea2;
           
            ga1 = gdata->allele_1[gdId[i]];
            ga2 = gdata->allele_2[gdId[i]];
            ea1 = esdata->_esi_allele1[edId[i]];
            ea2 = esdata->_esi_allele2[edId[i]];
            // use the allele in eQTL summary data as the reference allele. so we won't get the whole besd into memroy
            if( ea1 == ga1 && ea2 == ga2)
            {
               
                gdata->_include.push_back(gdId[i]);
                esdata->_esi_include.push_back(edId[i]);
            }
            else if(ea1 == ga2 && ea2 == ga1)
            {
                gdata->_include.push_back(gdId[i]);
                esdata->_esi_include.push_back(edId[i]);
                
                gdata->byz[gdId[i]]=-gdata->byz[gdId[i]];
                
            }
        }
        logstr=itos(gdata->_include.size())+" SNPs are included after allele check. ";
        cout<<logstr<<endl;
        
    }
    
    void allele_check(eqtlInfo* etrait, eqtlInfo* esdata)
    {
        // get the common SNPs
        vector<string> slctSNPs;
        vector<string> cmmnSNPs;
        vector<int> gdId;
        vector<int> edId;
        cmmnSNPs.clear();
        edId.clear();
        string logstr="Performing allele check between GWAS summary dataset and eQTL summary dataset. ";
        cout<<logstr<<endl;
         vector<string> etsnp;
        vector<string> essnp;
        if(esdata->_esi_include.size()<esdata->_snpNum || etrait->_esi_include.size()<etrait->_snpNum)
        {
            for(int i=0;i<esdata->_esi_include.size();i++) essnp.push_back(esdata->_esi_rs[esdata->_esi_include[i]]);
            for(int i=0;i<etrait->_esi_include.size();i++) etsnp.push_back(etrait->_esi_rs[etrait->_esi_include[i]]);
            StrFunc::match_only(essnp, etsnp, gdId);
            if(gdId.empty()) throw("Error: no common SNPs found.");
            for(int i=0;i<gdId.size();i++) slctSNPs.push_back(etsnp[gdId[i]]);
        }else
        {
            StrFunc::match_only(esdata->_esi_rs, etrait->_esi_rs, gdId);
            if(gdId.empty()) throw("Error: no common SNPs found.");
            for(int i=0;i<gdId.size();i++) slctSNPs.push_back(etrait->_esi_rs[gdId[i]]);
        }
        
        
        //alleles check
        StrFunc::match(slctSNPs, esdata->_esi_rs, edId);
        gdId.clear();
        StrFunc::match(slctSNPs, etrait->_esi_rs, gdId);
        cmmnSNPs.clear();
        etrait->_esi_include.clear();
        esdata->_esi_include.clear();
        
        for (int i = 0; i<edId.size(); i++)
        {
            char ta1, ta2, ea1, ea2;
            
            ta1 = etrait->_esi_allele1[gdId[i]];
            ta2 = etrait->_esi_allele2[gdId[i]];
            ea1 = esdata->_esi_allele1[edId[i]];
            ea2 = esdata->_esi_allele2[edId[i]];
            // use the allele in eQTL summary data as the reference allele. so we won't get the whole besd into memroy
            if( ea1 == ta1 && ea2 == ta2)
            {
                
                etrait->_esi_include.push_back(gdId[i]);
                esdata->_esi_include.push_back(edId[i]);
            }
            else if(ea1 == ta2 && ea2 == ta1)
            {
                etrait->_esi_include.push_back(gdId[i]);
                esdata->_esi_include.push_back(edId[i]);
                
                if(etrait->_val.size()>0)
                {
                    int count=0;
                    for(int j=0;j<etrait->_rowid.size();j++)
                    {
                        if(etrait->_rowid[j]==gdId[i])
                        {
                            count++;
                            if(count & 1)
                                etrait->_val[j]=-etrait->_val[j];
                        }
                    }
                }
                else
                {
                    for(int j=0;j<etrait->_include.size();j++) if( etrait->_bxz[j][gdId[i]]+9 > 1e-6 ) etrait->_bxz[j][gdId[i]]=-etrait->_bxz[j][gdId[i]];
                }
                
            }
        }
        logstr=itos(etrait->_esi_include.size())+" SNPs are included after allele check. ";
        cout<<logstr<<endl;
        
    }
    
    void allele_check(gwasData* gdata1, gwasData* gdata2)
    {
        // get the common SNPs
        vector<string> slctSNPs;
        vector<string> cmmnSNPs;
        vector<int> gdId;
        vector<int> edId;
        cmmnSNPs.clear();
        edId.clear();
        string logstr="Performing allele check between GWAS summary dataset and eQTL summary dataset. ";
        cout<<logstr<<endl;
        
        StrFunc::match_only(gdata2->snpName, gdata1->snpName, gdId);
        if(gdId.empty()) throw("Error: no common SNPs found.");
        
        for(int i=0;i<gdId.size();i++) slctSNPs.push_back(gdata1->snpName[gdId[i]]);
        
        //alleles check
        StrFunc::match(slctSNPs, gdata2->snpName, edId);
        cmmnSNPs.clear();
        gdata1->_include.clear();
        gdata2->_include.clear();
        
        for (int i = 0; i<edId.size(); i++)
        {
            char ga1, ga2, ea1, ea2;
            
            ga1 = gdata1->allele_1[gdId[i]];
            ga2 = gdata1->allele_2[gdId[i]];
            ea1 = gdata2->allele_1[edId[i]];
            ea2 = gdata2->allele_2[edId[i]];
            // use the allele in eQTL summary data as the reference allele. so we won't get the whole besd into memroy
            if( ea1 == ga1 && ea2 == ga2)
            {
                
                gdata1->_include.push_back(gdId[i]);
                gdata2->_include.push_back(edId[i]);
            }
            else if(ea1 == ga2 && ea2 == ga1)
            {
                gdata1->_include.push_back(gdId[i]);
                gdata2->_include.push_back(edId[i]);
                
                gdata1->byz[gdId[i]]=-gdata1->byz[gdId[i]];
            }
        }
        logstr=itos(gdata1->_include.size())+" SNPs are included after allele check. ";
        cout<<logstr<<endl;
    }
    void allele_check(bInfo* bdata, eqtlInfo* etrait, eqtlInfo* esdata)
    {
        // get the common SNPs
        vector<string> slctSNPs;
        vector<string> b_snp_name;
        vector<string> cmmnSNPs;
        vector<int> bdId;
        vector<int> gdId;
        vector<int> edId;
        cmmnSNPs.clear();
        edId.clear();
        string logstr="Performing allele check among GWAS summary dataset, eQTL summary dataset and Plink dataset. ";
        cout<<logstr<<endl;
        
        vector<string> bsnp;
        vector<string> etsnp;
        vector<string> essnp;
        if(bdata->_include.size()< bdata->_snp_num || esdata->_esi_include.size()<esdata->_snpNum || etrait->_esi_include.size()<etrait->_snpNum )
        {
            for(int i=0;i<bdata->_include.size();i++) bsnp.push_back(bdata->_snp_name[bdata->_include[i]]);
            for(int i=0;i<etrait->_esi_include.size();i++) etsnp.push_back(etrait->_esi_rs[etrait->_esi_include[i]]);
            for(int i=0;i<esdata->_esi_include.size();i++) essnp.push_back(esdata->_esi_rs[esdata->_esi_include[i]]);
            StrFunc::match_only(bsnp, etsnp, edId);
            if(edId.empty()) throw("Error: no common SNPs between PLink data and eTrait data.");
            for(int i=0;i<edId.size();i++) cmmnSNPs.push_back(etsnp[edId[i]]);
            StrFunc::match_only(cmmnSNPs, essnp, edId);
            if(edId.empty()) throw("Error: no common SNPs found.");
            for(int i=0;i<edId.size();i++) edId[i]=esdata->_esi_include[edId[i]];
            for(int i=0;i<edId.size();i++) slctSNPs.push_back(esdata->_esi_rs[edId[i]]);
        }else
        {
            StrFunc::match_only(bdata->_snp_name, etrait->_esi_rs, edId);
            if(edId.empty()) throw("Error: no common SNPs between PLink data and GWAS data.");
            for(int i=0;i<edId.size();i++) cmmnSNPs.push_back(etrait->_esi_rs[edId[i]]);
            edId.clear();
            StrFunc::match_only(cmmnSNPs, esdata->_esi_rs, edId);
            if(edId.empty()) throw("Error: no common SNPs found.");
            for(int i=0;i<edId.size();i++) slctSNPs.push_back(esdata->_esi_rs[edId[i]]);
        }
        
        //slctSNPs is in the order as bdata. so bdId is in increase order. edId and gdId may not.
        
        //alleles check
        StrFunc::match(slctSNPs, bdata->_snp_name, bdId);
        StrFunc::match(slctSNPs, etrait->_esi_rs, gdId);
        cmmnSNPs.clear();
        bdata->_include.clear();
        etrait->_esi_include.clear();
        esdata->_esi_include.clear();
        
        for (int i = 0; i<edId.size(); i++)
        {
            char a1, a2, ta1, ta2, ea1, ea2;
            a1 = bdata->_allele1[bdId[i]];
            a2 = bdata->_allele2[bdId[i]];
            ta1 = etrait->_esi_allele1[gdId[i]];
            ta2 = etrait->_esi_allele2[gdId[i]];
            ea1 = esdata->_esi_allele1[edId[i]];
            ea2 = esdata->_esi_allele2[edId[i]];
         
            // use the allele in eQTL summary data "esdata" as the reference allele. so we won't get the whole besd into memroy
            if(ea1 == a1 &&  ea2 == a2)
            {
                if( ea1 == ta1 && ea2 == ta2)
                {
                    
                    bdata->_include.push_back(bdId[i]);
                    etrait->_esi_include.push_back(gdId[i]);
                    esdata->_esi_include.push_back(edId[i]);
                }
                else if(ea1 == ta2 && ea2 == ta1)
                {
                    bdata->_include.push_back(bdId[i]);
                    etrait->_esi_include.push_back(gdId[i]);
                    esdata->_esi_include.push_back(edId[i]);
                    
                    if(etrait->_val.size()>0)
                    {
                        int count=0;
                        for(int j=0;j<etrait->_rowid.size();j++)
                        {
                            if(etrait->_rowid[j]==gdId[i])
                            {
                                count++;
                                if(count & 1)
                                    etrait->_val[j]=-etrait->_val[j];
                            }
                        }
                    }
                    else
                    {
                        for(int j=0;j<etrait->_include.size();j++) if( etrait->_bxz[j][gdId[i]]+9 > 1e-6 ) etrait->_bxz[j][gdId[i]]=-etrait->_bxz[j][gdId[i]];
                    }
                }
            }
            else if(ea1 == a2 &&  ea2 == a1)
            {
                
                if( ea1 == ta1 && ea2 == ta2)
                {
                    bdata->_include.push_back(bdId[i]);
                    etrait->_esi_include.push_back(gdId[i]);
                    esdata->_esi_include.push_back(edId[i]);
                    
                    char tmpch=bdata->_ref_A[bdId[i]];
                    bdata->_ref_A[bdId[i]]=bdata->_other_A[bdId[i]];
                    bdata->_other_A[bdId[i]]=tmpch;
                    
                }
                else if(ea1 == ta2 && ea2 == ta1)
                {
                    bdata->_include.push_back(bdId[i]);
                    etrait->_esi_include.push_back(gdId[i]);
                    esdata->_esi_include.push_back(edId[i]);
                    if(etrait->_val.size()>0)
                    {
                        int count=0;
                        for(int j=0;j<etrait->_rowid.size();j++)
                        {
                            if(etrait->_rowid[j]==gdId[i])
                            {
                                count++;
                                if(count & 1) etrait->_val[j]=-etrait->_val[j];
                            }
                        }
                    }
                    else
                    {
                        for(int j=0;j<etrait->_include.size();j++) if( etrait->_bxz[j][gdId[i]]+9 > 1e-6 ) etrait->_bxz[j][gdId[i]]=-etrait->_bxz[j][gdId[i]];
                    }
                    char tmpch=bdata->_ref_A[bdId[i]];
                    bdata->_ref_A[bdId[i]]=bdata->_other_A[bdId[i]];
                    bdata->_other_A[bdId[i]]=tmpch;
                    
                }
            }
            
            
        }
        
        logstr=itos(bdata->_include.size())+" SNPs are included after allele check. ";
        cout<<logstr<<endl;
        
        // only update _snp_name_map which would be used in read bed file.
        map<string, int> id_map_buf(bdata->_snp_name_map);
        for(int i=0; i<bdata->_include.size(); i++) id_map_buf.erase(bdata->_snp_name[bdata->_include[i]]);
        map<string, int>::iterator iter;
        for(iter=id_map_buf.begin(); iter!=id_map_buf.end(); iter++) bdata->_snp_name_map.erase(iter->first);
    }

    void update_gwas(gwasData* gdata){
        
        
        vector<string> snpName(gdata->_include.size());
        char* allele_1=(char*)malloc((gdata->_include.size()+1)*sizeof(char));
        char* allele_2=(char*)malloc((gdata->_include.size()+1)*sizeof(char));
        double* freq=(double*)malloc(gdata->_include.size()*sizeof(double));
        double* byz=(double*)malloc(gdata->_include.size()*sizeof(double));
        double* seyz=(double*)malloc(gdata->_include.size()*sizeof(double));
        double* pvalue=(double*)malloc(gdata->_include.size()*sizeof(double));
        uint32_t* splSize=(uint32_t*)malloc(gdata->_include.size()*sizeof(uint32_t));
        
        gdata->snpNum=gdata->_include.size();
        for(int i=0;i<gdata->_include.size();i++ )
        {
            snpName[i]=gdata->snpName[gdata->_include[i]];
            allele_1[i]=gdata->allele_1[gdata->_include[i]];
            allele_2[i]=gdata->allele_2[gdata->_include[i]];
            freq[i]=gdata->freq[gdata->_include[i]];
            byz[i]=gdata->byz[gdata->_include[i]];
            seyz[i]=gdata->seyz[gdata->_include[i]];
        }
        
        free(gdata->allele_1);
        free(gdata->allele_2);
        free(gdata->freq);
        free(gdata->byz);
        free(gdata->seyz);
        free(gdata->pvalue);
        free(gdata->splSize);
        
        gdata->snpName=snpName;
        gdata->allele_1=allele_1;
        gdata->allele_2=allele_2;
        gdata->freq=freq;
        gdata->byz=byz;
        gdata->seyz=seyz;
        gdata->pvalue = pvalue;
        gdata->splSize=splSize;
             
        gdata->allele_1[gdata->_include.size()]='\0';
        gdata->allele_2[gdata->_include.size()]='\0';
        
        for(int i=0;i<gdata->snpNum;i++) gdata->_include[i]=i;
        
    }
    
    void update_esd(eqtlInfo* etrait){
        
        
       
        
    }

    void make_XMat(bInfo* bdata,vector<uint32_t> &snpids, MatrixXd &X, bool minus_2p) {
        // Eigen is column-major by default. here row of X is individual, column of X is SNP.
        uint64_t snpNum=snpids.size();
        X.resize(bdata->_keep.size(),snpNum);
        #pragma omp parallel for
        for (int i = 0; i < snpNum ; i++)
        {
            uint32_t snpid=snpids[i];
            for (int j = 0; j < bdata->_keep.size() ; j++)
            {
                
                if (!bdata->_snp_1[bdata->_include[snpid]][bdata->_keep[j]] || bdata->_snp_2[bdata->_include[snpid]][bdata->_keep[j]])
                {
                    if (bdata->_allele1[bdata->_include[snpid]] == bdata->_ref_A[bdata->_include[snpid]]) X(j,i)= bdata->_snp_1[bdata->_include[snpid]][bdata->_keep[j]] + bdata->_snp_2[bdata->_include[snpid]][bdata->_keep[j]];
                    else X(j,i)= 2.0 - (bdata->_snp_1[bdata->_include[snpid]][bdata->_keep[j]] + bdata->_snp_2[bdata->_include[snpid]][bdata->_keep[j]]);
                } else X(j,i) = bdata->_mu[bdata->_include[snpid]];
                if (minus_2p) X(j,i) -= bdata->_mu[bdata->_include[snpid]];
                
            }
        }
    }
    void cor_calc(MatrixXd &LD, MatrixXd &X)
    {
        long size=X.cols();
        long n=X.rows();
        VectorXd tmpX(size);
        VectorXd tmpX2(size);
        #pragma omp parallel for
        for(int i=0;i<size;i++){
            tmpX[i]=X.col(i).sum();
            tmpX2[i]=X.col(i).dot(X.col(i));
        }
        LD.noalias()=X.transpose()*X;
        VectorXd tmpXX=(sqrt(tmpX2.array()*n-tmpX.array()*tmpX.array())).matrix();
        LD = (LD*n-tmpX*tmpX.transpose()).array()/ (tmpXX*tmpXX.transpose()).array();
        
    }
    void update_geIndx(bInfo* bdata, gwasData* gdata, eqtlInfo* esdata)
    {
        vector<uint32_t> tmpIdx1;
        vector<int> tmpIdx2;
        for (int i = 0; i < bdata->_include.size(); i++)
        {
            tmpIdx1.push_back(gdata->_include[bdata->_include[i]]);
            tmpIdx2.push_back(esdata->_esi_include[bdata->_include[i]]);
        }
        gdata->_include.clear();
        esdata->_esi_include.clear();
        gdata->_include = tmpIdx1;
        esdata->_esi_include = tmpIdx2;
    }
    void update_geIndx(bInfo* bdata, eqtlInfo* etrait, eqtlInfo* esdata)
    {
        vector<int> tmpIdx1;
        vector<int> tmpIdx2;
        for (int i = 0; i < bdata->_include.size(); i++)
        {
            tmpIdx1.push_back(etrait->_esi_include[bdata->_include[i]]);
            tmpIdx2.push_back(esdata->_esi_include[bdata->_include[i]]);
        }
        etrait->_esi_include.clear();
        esdata->_esi_include.clear();
        etrait->_esi_include = tmpIdx1;
        esdata->_esi_include = tmpIdx2;
    }


    void read_smaslist(vector<string> &smasNames, string eqtlsmaslstName)
    {
        ifstream smas(eqtlsmaslstName.c_str());
        if (!smas) throw ("Error: can not open the file [" + eqtlsmaslstName + "] to read.");
        cout << "Reading eQTL summary file names from [" + eqtlsmaslstName + "]." << endl;
        char buf[MAX_LINE_SIZE];
        while (smas.getline(buf, MAX_LINE_SIZE))
        {
            string tmpStr;
            istringstream iss(buf);
            iss >> tmpStr;
            smasNames.push_back(tmpStr);
        }
        cout << smasNames.size()<<" eQTL summary file names are included from [" + eqtlsmaslstName + "]." << endl;
        smas.close();
    }
    void combine_esi(eqtlInfo* eqtlinfo, vector<string> &smasNames)
    {
        eqtlinfo->_esi_chr.clear();
        eqtlinfo->_esi_rs.clear();
        eqtlinfo->_esi_gd.clear();
        eqtlinfo->_esi_bp.clear();
        eqtlinfo->_esi_allele1.clear();
        eqtlinfo->_esi_allele2.clear();
        eqtlinfo->_esi_include.clear();
        eqtlinfo->_snp_name_map.clear();
        long size = 0, counter = 0;
        
        for (int i = 0; i < smasNames.size(); i++)
        {
            string esifile = smasNames[i]+".esi";
            ifstream esi(esifile.c_str());
            if (!esi) throw ("Error: can not open the file [" + esifile + "] to read.");
            cout << "Reading eQTL SNP information from [" + esifile + "]." << endl;
            
            char buf[MAX_LINE_SIZE];
            int lineNum(0);
            while (!esi.eof())
            {
                esi.getline(buf, MAX_LINE_SIZE);
                lineNum++;
            }
            if (buf[0] == '\0') lineNum--;
            cout << lineNum << " SNPs to be included from [" + esifile + "]." << endl;
            
            esi.clear(ios::goodbit);
            esi.seekg(0, ios::beg);
            for (int j = 0; j<lineNum; j++)
            {
                string tmpStr;
                esi.getline(buf, MAX_LINE_SIZE);
                istringstream iss(buf);
                iss >> tmpStr;
                int chrtmp = atoi(tmpStr.c_str());
                iss >> tmpStr;
                
                eqtlinfo->_snp_name_map.insert(pair<string, int>(tmpStr,counter));
                if (size < eqtlinfo->_snp_name_map.size())
                {
                    size = eqtlinfo->_snp_name_map.size();
                    eqtlinfo->_esi_chr.push_back(chrtmp);
                    eqtlinfo->_esi_rs.push_back(tmpStr);
                    iss >> tmpStr;
                    eqtlinfo->_esi_gd.push_back(atoi(tmpStr.c_str()));
                    iss >> tmpStr;
                    eqtlinfo->_esi_bp.push_back(atoi(tmpStr.c_str()));
                    iss >> tmpStr;
                    StrFunc::to_upper(tmpStr);
                    eqtlinfo->_esi_allele1.push_back(tmpStr.c_str()[0]);
                    iss >> tmpStr;
                    StrFunc::to_upper(tmpStr);
                    eqtlinfo->_esi_allele2.push_back(tmpStr.c_str()[0]);
                    eqtlinfo->_esi_include.push_back(counter++);
                }
            }
            esi.close();
        }
        eqtlinfo->_snpNum =eqtlinfo->_esi_include.size();
        cout <<"Total " <<eqtlinfo->_snpNum << " SNPs to be included! " << endl;
    }
    
    void combine_epi_esd(eqtlInfo* eqtlinfo, vector<string> &smasNames, string outFileName)
    {
        eqtlinfo->_cols.push_back(0);
        vector<uint32_t> duplicatedid;
        for (int i = 0; i < smasNames.size(); i++)
        {
            string esifile = smasNames[i] + ".esi";
            string epifile = smasNames[i] + ".epi";
            string esdfile = smasNames[i] + ".besd";
            eqtlInfo tmp_info;
            read_esifile(&tmp_info, esifile);
            read_epifile(&tmp_info, epifile);
            read_besdfile(&tmp_info, esdfile);
            for (int j = 0; j < tmp_info._probNum; j++)
            {
                duplicatedid.clear();
                uint64_t beta_start = tmp_info._cols[j << 1];
                uint64_t se_start = tmp_info._cols[1 + (j << 1)];
                int numsnps = se_start - beta_start;
                for (int k = 0; k < numsnps; k++)
                {
                    uint32_t ge_rowid = tmp_info._rowid[beta_start + k];
                    string snpname = tmp_info._esi_rs[ge_rowid];
                    map<string, int>::iterator iter;
                    iter = eqtlinfo->_snp_name_map.find(snpname);
                    if (iter == eqtlinfo->_snp_name_map.end()) throw("Unknown error!");
                    uint32_t sid = iter->second;
                    eqtlinfo->_rowid.push_back(sid);
                    duplicatedid.push_back(sid);
                    eqtlinfo->_val.push_back(tmp_info._val[beta_start + k]);
                }
                for (int k = 0; k < numsnps; k++)
                {
                    uint32_t ge_rowid = tmp_info._rowid[se_start + k];
                    uint32_t sid = duplicatedid[k];
                    eqtlinfo->_rowid.push_back(sid);
                    eqtlinfo->_val.push_back(tmp_info._val[se_start + k]);
                }
                uint64_t lastcolnum = eqtlinfo->_cols[eqtlinfo->_cols.size() - 1] ;
                eqtlinfo->_cols.push_back(lastcolnum + numsnps);
                eqtlinfo->_cols.push_back(lastcolnum + (numsnps<<1));
                
                eqtlinfo->_epi_bp.push_back(tmp_info._epi_bp[j]);
                eqtlinfo->_epi_chr.push_back(tmp_info._epi_chr[j]);
                eqtlinfo->_epi_gd.push_back(tmp_info._epi_gd[j]);
                eqtlinfo->_epi_gene.push_back(tmp_info._epi_gene[j]);
                eqtlinfo->_epi_orien.push_back(tmp_info._epi_orien[j]);
                eqtlinfo->_epi_prbID.push_back(tmp_info._epi_prbID[j]);
            }
        }
        eqtlinfo->_valNum = eqtlinfo->_val.size();
        eqtlinfo->_probNum = eqtlinfo->_epi_prbID.size();
        for (int i = 0; i < eqtlinfo->_probNum; i++) eqtlinfo->_include.push_back(i);
        cout << "Total "<<eqtlinfo->_probNum << " probes and "<< eqtlinfo->_snpNum << " SNPs have been combined." << endl;
        
        filter_probe_null(eqtlinfo); // at the same time, reset the vector _include
        cout << "\nsaving eQTL data..." << endl;
        string esdfile = outFileName + ".esi";
        ofstream smr(esdfile.c_str());
        if (!smr) throw ("Error: can not open the esi file " + esdfile + " to save!");
        for (int i = 0; i <eqtlinfo->_snpNum; i++) {
            smr << eqtlinfo->_esi_chr[i] << '\t' << eqtlinfo->_esi_rs[i] << '\t' << eqtlinfo->_esi_gd[i] << '\t' << eqtlinfo->_esi_bp[i] << '\t' << eqtlinfo->_esi_allele1[i] << '\t' << eqtlinfo->_esi_allele2[i] << '\n';
        }
        smr.close();
        cout << eqtlinfo->_snpNum << " SNPs have been saved in the file [" + esdfile + "]." << endl;
        
        esdfile = string(outFileName) + ".epi";
        smr.open(esdfile.c_str());
        if (!smr) throw ("Error: can not open the epi file " + esdfile + " to save!");
        for (int i = 0; i <eqtlinfo->_include.size(); i++) {
            smr << eqtlinfo->_epi_chr[eqtlinfo->_include[i]] << '\t' << eqtlinfo->_epi_prbID[eqtlinfo->_include[i]] << '\t' << eqtlinfo->_epi_gd[eqtlinfo->_include[i]] << '\t' << eqtlinfo->_epi_bp[eqtlinfo->_include[i]] << '\t' << eqtlinfo->_epi_gene[eqtlinfo->_include[i]] << '\t' << eqtlinfo->_epi_orien[eqtlinfo->_include[i]] << '\n';
        }
        smr.close();
        cout << eqtlinfo->_probNum << " probes have been saved in the file [" + esdfile + "]." << endl;
        
        esdfile = string(outFileName) + ".besd";
        FILE * smrbesd;
        smrbesd = fopen(esdfile.c_str(), "wb");
        uint64_t colSize = sizeof(uint64_t)*((eqtlinfo->_include.size() << 1) + 1);
        uint64_t rowSize = sizeof(uint32_t)*eqtlinfo->_valNum;
        uint64_t valSize = sizeof(float)*eqtlinfo->_valNum;
        uint64_t valNum = eqtlinfo->_valNum;
        uint64_t bufsize = sizeof(float) + sizeof(uint64_t) + colSize + rowSize + valSize;
        
        char* buffer = (char*)malloc(sizeof(char)*bufsize);
        memset(buffer, 0, sizeof(char)*bufsize);
        float ftype = SPARSE_FILE_TYPE_3;
        memcpy(buffer, &ftype, sizeof(float));
        char* wptr = buffer + sizeof(float);
        memcpy(wptr, &valNum, sizeof(uint64_t));
        wptr += sizeof(uint64_t);
        uint64_t* uptr = (uint64_t*)wptr; *uptr++ = 0;
        for (int i = 0; i<eqtlinfo->_include.size(); i++)
        {
            *uptr++ = eqtlinfo->_cols[(eqtlinfo->_include[i] << 1) + 1];
            *uptr++ = eqtlinfo->_cols[eqtlinfo->_include[i] + 1 << 1];
        }
        wptr += colSize;
        memcpy(wptr, &eqtlinfo->_rowid[0], rowSize);
        wptr += rowSize;
        memcpy(wptr, &eqtlinfo->_val[0], valSize);
        fwrite(buffer, sizeof(char), bufsize, smrbesd);
        free(buffer);
        fclose(smrbesd);
        
        cout << "Beta values and SE values for " << eqtlinfo->_include.size() << " Probes and " << eqtlinfo->_snpNum << " SNPs have been saved in the binary file [" + esdfile + "]." << endl;
        
    }
    
    void combine_epi(eqtlInfo* eqtlinfo, vector<string> &smasNames)
    {
        eqtlinfo->_epi_bp .clear();
        eqtlinfo->_epi_chr.clear();
        eqtlinfo->_epi_gd.clear();
        eqtlinfo->_epi_gene.clear();
        eqtlinfo->_epi_orien.clear();
        eqtlinfo->_epi_prbID.clear();
        eqtlinfo->_include.clear();
        eqtlinfo->_probe_name_map.clear();
        long size = 0, counter = 0;
        
        for (int i = 0; i < smasNames.size(); i++)
        {
            string epifile = smasNames[i]+".epi";
            ifstream epi(epifile.c_str());
            if (!epi) throw ("Error: can not open the file [" + epifile + "] to read.");
            cout << "Reading eQTL SNP information from [" + epifile + "]." << endl;
            
            char buf[MAX_LINE_SIZE];
            int lineNum(0);
            while (!epi.eof())
            {
                epi.getline(buf, MAX_LINE_SIZE);
                lineNum++;
            }
            if (buf[0] == '\0') lineNum--;
            cout << lineNum << " Probes to be included from [" + epifile + "]." << endl;
            
            epi.clear(ios::goodbit);
            epi.seekg(0, ios::beg);
            for (int j = 0; j<lineNum; j++)
            {
                string tmpStr;
                epi.getline(buf, MAX_LINE_SIZE);
                istringstream iss(buf);
                iss >> tmpStr;
                int chrtmp = atoi(tmpStr.c_str());
                iss >> tmpStr;
                
                eqtlinfo->_probe_name_map.insert(pair<string, int>(tmpStr,counter));
                if (size == eqtlinfo->_probe_name_map.size())
                {
                    cout << " Warning: probe "<< tmpStr <<" is duplicate!" << endl;
                }
                size = eqtlinfo->_probe_name_map.size();
                eqtlinfo->_epi_chr.push_back(chrtmp);
                eqtlinfo->_epi_prbID.push_back(tmpStr);
                iss >> tmpStr;
                eqtlinfo->_epi_gd.push_back(atoi(tmpStr.c_str()));
                iss >> tmpStr;
                eqtlinfo->_epi_bp.push_back(atoi(tmpStr.c_str()));
                iss >> tmpStr;
                eqtlinfo->_epi_gene.push_back(tmpStr.c_str());
                iss >> tmpStr;
                eqtlinfo->_epi_orien.push_back(tmpStr.c_str()[0]);
                eqtlinfo->_include.push_back(counter++);
            }
            epi.close();
        }
        eqtlinfo->_probNum =eqtlinfo->_include.size();
        cout <<"Total " <<eqtlinfo->_probNum << " Probes to be included! " << endl;
    }

    void combine_epi_esd_dense(eqtlInfo* eqtlinfo, vector<string> &smasNames, string outFileName)
    {
        uint64_t bsize=(eqtlinfo->_include.size()*eqtlinfo->_esi_include.size()<<1)+1;
        float* buffer=(float*)malloc (sizeof(float)*bsize);
        memset(buffer,0,sizeof(float)*bsize);
        float* ptr=buffer;
        *ptr++=0.0;
        for (int i = 0; i < smasNames.size(); i++)
        {
            string esifile = smasNames[i] + ".esi";
            string epifile = smasNames[i] + ".epi";
            string esdfile = smasNames[i] + ".besd";
            eqtlInfo tmp_info;
            read_esifile(&tmp_info, esifile);
            read_epifile(&tmp_info, epifile);
            read_besdfile(&tmp_info, esdfile);
            if(tmp_info._esi_include.size()==eqtlinfo->_esi_include.size())
            {
                // we suppose the snps in tmp_info and eqtlinfo are in the same order. if not match should be exploited.
                long snp_num=tmp_info._esi_include.size();
                for (int j = 0; j < tmp_info._include.size(); j++)
                {
                    memcpy(ptr,&tmp_info._bxz[tmp_info._include[j]][0],sizeof(float)*snp_num);
                    ptr+=snp_num;
                    memcpy(ptr,&tmp_info._sexz[tmp_info._include[j]][0],sizeof(float)*snp_num);
                    ptr+=snp_num;
                }
            }
            else
            {
                vector<int> idx;
                match_only(tmp_info._esi_rs,eqtlinfo->_esi_rs,idx);
                long snp_num=eqtlinfo->_esi_include.size();
                for(int k=0;k<(snp_num<<1);k++) *(ptr+k)=-9;
                for (int j = 0; j < tmp_info._include.size(); j++)
                {
                    for(int k=0;k<idx.size();k++)  *(ptr+idx[k])=tmp_info._bxz[tmp_info._include[j]][k];
                    ptr+=snp_num;
                    for(int k=0;k<idx.size();k++)  *(ptr+idx[k])=tmp_info._sexz[tmp_info._include[j]][k];
                    ptr+=snp_num;
                }
            }
            
        }
       
        cout << "Total "<<eqtlinfo->_probNum << " probes and "<< eqtlinfo->_snpNum << " SNPs have been combined." << endl;
      
        cout << "\nsaving eQTL data..." << endl;
        string esdfile = outFileName + ".esi";
        ofstream smr(esdfile.c_str());
        if (!smr) throw ("Error: can not open the esi file " + esdfile + " to save!");
        for (int i = 0; i <eqtlinfo->_snpNum; i++) {
            smr << eqtlinfo->_esi_chr[i] << '\t' << eqtlinfo->_esi_rs[i] << '\t' << eqtlinfo->_esi_gd[i] << '\t' << eqtlinfo->_esi_bp[i] << '\t' << eqtlinfo->_esi_allele1[i] << '\t' << eqtlinfo->_esi_allele2[i] << '\n';
        }
        smr.close();
        cout << eqtlinfo->_snpNum << " SNPs have been saved in the file [" + esdfile + "]." << endl;
        
        esdfile = string(outFileName) + ".epi";
        smr.open(esdfile.c_str());
        if (!smr) throw ("Error: can not open the epi file " + esdfile + " to save!");
        for (int i = 0; i <eqtlinfo->_include.size(); i++) {
            smr << eqtlinfo->_epi_chr[eqtlinfo->_include[i]] << '\t' << eqtlinfo->_epi_prbID[eqtlinfo->_include[i]] << '\t' << eqtlinfo->_epi_gd[eqtlinfo->_include[i]] << '\t' << eqtlinfo->_epi_bp[eqtlinfo->_include[i]] << '\t' << eqtlinfo->_epi_gene[eqtlinfo->_include[i]] << '\t' << eqtlinfo->_epi_orien[eqtlinfo->_include[i]] << '\n';
        }
        smr.close();
        cout << eqtlinfo->_probNum << " probes have been saved in the file [" + esdfile + "]." << endl;
        
        esdfile = string(outFileName) + ".besd";
        FILE * smrbesd;
        smrbesd = fopen(esdfile.c_str(), "wb");
       
        fwrite(buffer, sizeof(float), bsize, smrbesd);
        free(buffer);
        fclose(smrbesd);
        cout << "Beta values and SE values for " << eqtlinfo->_include.size() << " Probes and " << eqtlinfo->_snpNum << " SNPs have been saved in the binary file [" + esdfile + "]." << endl;
       
    }
    void combine_epi_esd_dense2sparse(eqtlInfo* eqtlinfo, vector<string> &smasNames, string outFileName,int cis_itvl, int trans_itvl, float transThres, float restThres)
    {
        cout<<"Transforming dense file to sparse file ..."<<endl;
        vector<uint64_t> cols;
        vector<uint32_t> rowids;
        vector<float> val;
        FILE* logfile=NULL;
        string logfname = string(outFileName)+".summary";
        logfile=fopen(logfname.c_str(), "w");
        if (!(logfile)) {
            printf("Error: Failed to open log file.\n");
            exit(1);
        }
        string logstr="--cis-itvl:\t"+itos(cis_itvl)+"Kb\n--trans-itvl:\t"+itos(trans_itvl)+"Kb\n--trans-thres:\t"+dtos(transThres)+"\n--rest-thres:\t"+dtos(restThres)+"\n";
        logstr+="cis region is represent as [Chr, Start bp, End bp, nsnp,trans_touch_cis]; trans region is represent as <Chr, Start bp, End bp, nsnp, extend, merge>.\n";
        logstr+="trans_touch_cis: cis region and trans region are overlapped then merged into one big cis region.\n";
        logstr+="extend: trans region contains more than two SNPs that meet the trans threshold.\n";
        logstr+="merge: two or more trans regions are overlapped and megered into one big trans region.\n\n";
        logstr+="{ProbeID,ProbeBP}\t[Chr,cis_startBP,cis_endBP,NumSNPs,merged]\t<Chr,trans_startBP,trans_endBP,NumSNPs,merged>\tNumSNPs beyond cis and trans\n";
        fputs(logstr.c_str(),logfile);
        fflush(logfile);
        cis_itvl=cis_itvl*1000;
        trans_itvl=trans_itvl*1000;
        cols.push_back(0);
        string pretransPrbId="";
        // for log file
        vector<int> tran_chr;
        vector<uint64_t> lowerBp;
        vector<uint64_t> upperBp;
        vector<uint64_t> nsnp;
        vector<bool> extend;
        vector<bool> merge;
        vector<int> cis_idx;

        for (int i = 0; i < smasNames.size(); i++)
        {
            string esifile = smasNames[i] + ".esi";
            string epifile = smasNames[i] + ".epi";
            string esdfile = smasNames[i] + ".besd";
            eqtlInfo tmp_info;
            read_esifile(&tmp_info, esifile);
            read_epifile(&tmp_info, epifile);
            read_besdfile(&tmp_info, esdfile);
            if(tmp_info._valNum>0) throw("Error: please input dense format eQTL summary data file.");
            if(tmp_info._esi_include.size()==eqtlinfo->_esi_include.size())
            {
                // we suppose the snps in tmp_info and eqtlinfo are in the same order. if not match should be exploited.
                
                for (int j = 0; j < tmp_info._include.size(); j++)
                {
                    uint32_t snpCount=0;
                    long mapsize=0;
                    map<int,uint32_t> snp_id_map;
                    vector<int> esi_include;
                    int probchr=tmp_info._epi_chr[j];
                    int probbp= tmp_info._epi_bp[j];
                    uint64_t cisuperBounder= probbp+cis_itvl;
                    uint64_t cislowerBounder=((probbp-cis_itvl>0)?(probbp-cis_itvl):0);
                    uint64_t cisNum=0;
                    uint64_t otherSlctNum=0;
                    
                    // for log file
                    tran_chr.clear();
                    lowerBp.clear();
                    upperBp.clear();
                    nsnp.clear();
                    extend.clear();
                    merge.clear();
                    
                    uint64_t realcisUBp=0;
                    uint64_t realcisLBp=0;
                    uint64_t realcisNum=0;
                    
                    bool trans_touch_cis_upper = false;
                    bool trans_touch_cis_lower = false;
                    //
                    cis_idx.clear();
                    for(int k=0;k<tmp_info._snpNum;k++)
                    {
                        if(abs(tmp_info._sexz[j][k]+9)<1e-6) continue;
                        
                        double zsxz=tmp_info._bxz[j][k]/tmp_info._sexz[j][k];
                        double pxz=pchisq(zsxz*zsxz, 1);
                        
                        if(tmp_info._esi_chr[k] == probchr && tmp_info._esi_bp[k]<=cisuperBounder && tmp_info._esi_bp[k]>=cislowerBounder && abs(tmp_info._sexz[j][k]+9)>1e-6)
                        {
                            snp_id_map.insert(pair<int, int>(k,snpCount++));
                            if(mapsize<snp_id_map.size()){
                                esi_include.push_back(k);
                                mapsize=snp_id_map.size();
                            }
                            
                            cis_idx.push_back(k);
                            cisNum++;
                        }
                        else if(pxz<transThres)
                        {
                            uint64_t transNum=0;
                            uint32_t curChr=tmp_info._esi_chr[k];
                            snp_id_map.insert(pair<int, int>(k,snpCount++));
                            if(mapsize<snp_id_map.size()){
                                esi_include.push_back(k);
                                mapsize=snp_id_map.size();
                            }
                            long transbp=tmp_info._esi_bp[k];
                            long translowerBounder=((transbp-trans_itvl>0)?(transbp-trans_itvl):0);
                            long transuperBounder=transbp+trans_itvl;
                            bool extended=false;
                            bool merged=false;
                            
                            // assume esi is sorted
                            int startptr=k-1;
                            while(startptr>=0 && curChr == tmp_info._esi_chr[startptr] && transbp-tmp_info._esi_bp[startptr]<=trans_itvl)
                            {
                                if(abs(tmp_info._sexz[j][startptr]+9)>1e-6)
                                {
                                    translowerBounder=tmp_info._esi_bp[startptr];
                                    snp_id_map.insert(pair<int, int>(startptr,snpCount++));
                                    if(mapsize<snp_id_map.size()){
                                        esi_include.push_back(startptr);
                                        mapsize=snp_id_map.size();
                                    }
                                    if(upperBp.size()>0 && translowerBounder<=upperBp[upperBp.size()-1]) //trans region merges
                                    {
                                        merged=true;
                                        transNum=transNum+nsnp[nsnp.size()-1];
                                        break;
                                    }
                                    if(tmp_info._esi_chr[startptr] == probchr && translowerBounder<=cisuperBounder && translowerBounder>=cislowerBounder) // trans touches cis region
                                    {
                                        trans_touch_cis_upper=true;
                                        transNum=transNum+cis_idx.size();
                                        translowerBounder=tmp_info._esi_bp[cis_idx[0]];
                                        break;
                                    }
                                    transNum++;
                                }
                                startptr--;
                            }
                            startptr=k+1;
                            while (startptr<tmp_info._snpNum && curChr == tmp_info._esi_chr[startptr] && tmp_info._esi_bp[startptr] - transbp <= trans_itvl)
                            {
                                if(abs(tmp_info._sexz[j][startptr]+9)>1e-6)
                                {
                                    transuperBounder=tmp_info._esi_bp[startptr];
                                    if(tmp_info._esi_chr[startptr] == probchr && transuperBounder>=cislowerBounder && transuperBounder<=cisuperBounder) // trans touches cis region
                                    {
                                        trans_touch_cis_lower=true;
                                        cisNum=transNum;
                                        while(tmp_info._esi_chr[startptr] == probchr && tmp_info._esi_bp[startptr]<=cisuperBounder && tmp_info._esi_bp[startptr]>=cislowerBounder )
                                        {
                                            if(abs(tmp_info._sexz[j][startptr]+9)>1e-6)
                                            {
                                                transuperBounder=tmp_info._esi_bp[startptr];
                                                snp_id_map.insert(pair<int, int>(startptr,snpCount++));
                                                if(mapsize<snp_id_map.size()){
                                                    esi_include.push_back(startptr);
                                                    mapsize=snp_id_map.size();
                                                }
                                                cisNum++;
                                            }
                                            startptr++;
                                        }
                                        k=startptr; // because cis_itvl >= trans_itvl. so here startptr can't satisfy the outer while loop anymore. j would jump to outer for loop
                                        break; // I think it doesnot matter with or without break here.
                                    }
                                    else
                                    {
                                        snp_id_map.insert(pair<int, int>(startptr,snpCount++));
                                        if(mapsize<snp_id_map.size()){
                                            esi_include.push_back(startptr);
                                            mapsize=snp_id_map.size();
                                        }
                                        double zsxz_tmp=tmp_info._bxz[j][startptr]/tmp_info._sexz[j][startptr];
                                        double pxz_tmp=pchisq(zsxz_tmp*zsxz_tmp, 1);
                                        if(pxz_tmp<transThres) // trans region extends
                                        {
                                            extended=true;
                                            k=startptr;
                                            transbp=tmp_info._esi_bp[k];
                                        }
                                        transNum++;
                                        k=startptr;
                                    }
                                }
                                startptr++;
                            }
                            if(!trans_touch_cis_lower && !trans_touch_cis_upper)
                            {
                                if(merged)
                                {
                                    upperBp[upperBp.size()-1]=transuperBounder;
                                    nsnp[nsnp.size()-1]=transNum;
                                    extend[nsnp.size()-1]=extended;
                                    merge[nsnp.size()-1]=merged;
                                }
                                else
                                {
                                    tran_chr.push_back(curChr);
                                    upperBp.push_back(transuperBounder);
                                    lowerBp.push_back(translowerBounder);
                                    nsnp.push_back(transNum);
                                    extend.push_back(extended);
                                    merge.push_back(merged);
                                }
                                
                            }
                            else
                            {
                                realcisNum=transNum;
                                realcisUBp=transuperBounder;
                                realcisLBp=translowerBounder;
                            }
                        }
                        else if(pxz<restThres)
                        {
                            snp_id_map.insert(pair<int, int>(k,snpCount++));
                            if(mapsize<snp_id_map.size()){
                                esi_include.push_back(k);
                                mapsize=snp_id_map.size();
                            }
                            otherSlctNum++;
                        }
                    }
                    if(cis_idx.size()>0 && !trans_touch_cis_lower && !trans_touch_cis_upper) // in case of the probe on other chr has no trans or otherSlct on current chr
                    {
                        realcisNum=cis_idx.size();
                        realcisLBp=tmp_info._esi_bp[cis_idx[0]];
                        realcisUBp=tmp_info._esi_bp[cis_idx[realcisNum-1]];
                    }
                    // log file
                    if(realcisNum || nsnp.size() || otherSlctNum)
                    {
                        logstr="{"+tmp_info._epi_prbID[j]+","+atos(tmp_info._epi_bp[j])+"}\t";
                        if(realcisNum)
                        {
                            string overlapflg="F";
                            if(trans_touch_cis_lower || trans_touch_cis_upper) overlapflg="T";
                            logstr+= "["+atos(probchr)+","+ atos(realcisLBp)+","+atos(realcisUBp)+","+atos(realcisNum)+","+overlapflg+"]\t";
                        }else logstr+="[]\t";
                        if(nsnp.size())
                        {
                            for(int h=0;h<nsnp.size();h++)
                            {
                                string mergeflg="F";
                                if(extend[h] || merge[h]) mergeflg="T";
                                logstr+= "<"+atos(tran_chr[h])+","+ atos(lowerBp[h])+","+atos(upperBp[h])+","+atos(nsnp[h])+","+mergeflg+">\t";
                            }
                        }else logstr+="<>\t";
                        
                        if(otherSlctNum) logstr+="("+atos(otherSlctNum)+")\n";
                        else logstr+="()\n";
                        
                        fputs(logstr.c_str(),logfile);
                        fflush(logfile);
                    }
                    
                    //
                    sort( esi_include.begin(), esi_include.end() );
                    vector<int> ::iterator it=unique(esi_include.begin(),esi_include.end());
                    esi_include.erase( it, esi_include.end() );
                    
                    for(int k=0;k<esi_include.size();k++)
                    {
                        val.push_back(tmp_info._bxz[j][esi_include[k]]);
                        rowids.push_back(esi_include[k]);
                    }
                    for(int k=0;k<esi_include.size();k++)
                    {
                        val.push_back(tmp_info._sexz[j][esi_include[k]]);
                        rowids.push_back(esi_include[k]);
                    }
                    uint64_t real_num=esi_include.size();
                    uint64_t curnum=cols[cols.size()-1];
                    cols.push_back(real_num+curnum);
                    cols.push_back((real_num<<1)+curnum);
                }
               
            }
            else
            {
                vector<int> idx;
                match_only(tmp_info._esi_rs,eqtlinfo->_esi_rs,idx);
                cout<<"Not finished this part!"<<endl;
                exit(1);
            }
            
        }
        
        uint64_t colSize=sizeof(uint64_t)*cols.size();
        uint64_t rowSize=sizeof(uint32_t)*rowids.size();
        uint64_t valSize=sizeof(float)*val.size();
        uint64_t valNum=val.size();
        uint64_t bufsize=sizeof(float)+sizeof(uint64_t)+colSize+rowSize+valSize;
        char* buffer=(char*)malloc (bufsize*sizeof(char));
        char* wptr=buffer;
        float ftype=SPARSE_FILE_TYPE_3;
        memcpy(wptr,&ftype,sizeof(float));
        wptr+=sizeof(float);
        memcpy(wptr,&valNum,sizeof(uint64_t));
        wptr+=sizeof(uint64_t);
        memcpy(wptr,&cols[0],colSize);
        wptr+=colSize;
        memcpy(wptr,&rowids[0],rowSize);
        wptr+=rowSize;
        memcpy(wptr,&val[0],valSize);

        cout << "Total "<<eqtlinfo->_probNum << " probes and "<< eqtlinfo->_snpNum << " SNPs have been combined." << endl;
        
        cout << "\nsaving eQTL data..." << endl;
        string esdfile = outFileName + ".esi";
        ofstream smr(esdfile.c_str());
        if (!smr) throw ("Error: can not open the esi file " + esdfile + " to save!");
        for (int i = 0; i <eqtlinfo->_snpNum; i++) {
            smr << eqtlinfo->_esi_chr[i] << '\t' << eqtlinfo->_esi_rs[i] << '\t' << eqtlinfo->_esi_gd[i] << '\t' << eqtlinfo->_esi_bp[i] << '\t' << eqtlinfo->_esi_allele1[i] << '\t' << eqtlinfo->_esi_allele2[i] << '\n';
        }
        smr.close();
        cout << eqtlinfo->_snpNum << " SNPs have been saved in the file [" + esdfile + "]." << endl;
        
        esdfile = string(outFileName) + ".epi";
        smr.open(esdfile.c_str());
        if (!smr) throw ("Error: can not open the epi file " + esdfile + " to save!");
        for (int i = 0; i <eqtlinfo->_include.size(); i++) {
            smr << eqtlinfo->_epi_chr[eqtlinfo->_include[i]] << '\t' << eqtlinfo->_epi_prbID[eqtlinfo->_include[i]] << '\t' << eqtlinfo->_epi_gd[eqtlinfo->_include[i]] << '\t' << eqtlinfo->_epi_bp[eqtlinfo->_include[i]] << '\t' << eqtlinfo->_epi_gene[eqtlinfo->_include[i]] << '\t' << eqtlinfo->_epi_orien[eqtlinfo->_include[i]] << '\n';
        }
        smr.close();
        cout << eqtlinfo->_probNum << " probes have been saved in the file [" + esdfile + "]." << endl;
        
        
        esdfile = string(outFileName)+".besd";
        FILE * besd;
        besd = fopen (esdfile.c_str(), "wb");
        fwrite (buffer,sizeof(char), bufsize, besd);
        fclose(besd);
        if(logfile) fclose(logfile);
        free(buffer);
        cout<<"Beta values and SE values for "<<eqtlinfo->_include.size()<<" Probes and "<<eqtlinfo->_snpNum<<" SNPs have been saved in the binary file [" + esdfile + "]." <<endl;
        cout<<"Log information has been saved in the file [" + logfname + "]." <<endl;
    }



    void ld_calc_o2m(VectorXd &ld_v,long targetid, MatrixXd &X)
    {
        long size=X.cols();
        long n=X.rows();
        
        VectorXd tmpX(size);
        VectorXd tmpX2(size);
        VectorXd tmpXY(size);
        #pragma omp parallel for
        for(int i=0;i<size;i++){
            tmpX[i]=X.col(i).sum();
            tmpX2[i]=X.col(i).dot(X.col(i));
            tmpXY[i]=X.col(targetid).dot(X.col(i));
        }
        float tmpY=X.col(targetid).sum();;
        float tmpY2=X.col(targetid).dot(X.col(targetid));
        ld_v=(tmpXY*n-tmpX*tmpY).array()/sqrt((tmpX2.array()*n-tmpX.array()*tmpX.array())*(tmpY2*n-tmpY*tmpY));
    }
    void smr(char* outFileName, char* bFileName,char* gwasFileName, char* eqtlFileName, double maf,char* indilstName, char* snplstName,char* problstName,bool bFlag,double p_hetero,double ld_top,int m_hetero , char* indilst2remove, char* snplst2exclde, char* problst2exclde,double p_smr, char* refSNP, bool heidioffFlag, int cis_itvl,bool plotflg)
    {
        
        setNbThreads(thread_num);
        
        bInfo bdata;
        gwasData gdata;
        eqtlInfo esdata;
        double threshold= chi_val(1,p_hetero);
        bool heidiFlag=false;
        
        if(!heidioffFlag && bFileName == NULL ) throw("Error: please input Plink file for SMR analysis by the flag --bfile.");
        if(gwasFileName==NULL) throw("Error: please input GWAS summary data for SMR analysis by the flag --gwas-summary.");
        if(eqtlFileName==NULL) throw("Error: please input eQTL summary data for SMR analysis by the flag --eqtl-summary.");
        if(refSNP!=NULL) heidiFlag=true;
        read_gwas_data( &gdata, gwasFileName);
        read_esifile(&esdata, string(eqtlFileName)+".esi");
        if (snplstName != NULL) extract_eqtl_snp(&esdata, snplstName);
        if(snplst2exclde != NULL) exclude_eqtl_snp(&esdata, snplst2exclde); //no switch the place ahead of extract_eqtl_snp()
        if(!heidioffFlag)
        {
            read_famfile(&bdata, string(bFileName)+".fam");
            if(indilstName != NULL) keep_indi(&bdata,indilstName);
            if(indilst2remove != NULL) remove_indi(&bdata, indilst2remove);
            read_bimfile(&bdata, string(bFileName)+".bim");
            if(snplstName != NULL) extract_snp(&bdata, snplstName);
            if(snplst2exclde != NULL) exclude_snp(&bdata, snplst2exclde);
            allele_check(&bdata, &gdata, &esdata);
            read_bedfile(&bdata, string(bFileName)+".bed");
            if (bdata._mu.empty()) calcu_mu(&bdata);
            if (maf > 0)
            {
                filter_snp_maf(&bdata, maf);
                update_geIndx(&bdata, &gdata, &esdata);
            }

        }else
        {
             allele_check(&gdata, &esdata);
        }        
       
        update_gwas(&gdata);
        cout<<"Reading eQTL summary data..."<<endl;
        read_epifile(&esdata, string(eqtlFileName)+".epi");
        if(problstName != NULL) extract_prob(&esdata, problstName);
        if(problst2exclde != NULL) exclude_prob(&esdata, problst2exclde); //no switch the place ahead of extract_prob()
        if(bFlag) read_besdfile(&esdata, string(eqtlFileName)+".besd");
        else      read_esdfile(&esdata, string(eqtlFileName)+".esd");
        //test the SNP alignement
        //for(int i=0;i<bdata._include.size();i++)
        //    if(gdata.snpName[i]!=esdata._esi_rs[i] || bdata._snp_name[bdata._include[i]] != gdata.snpName[i] || bdata._snp_name[bdata._include[i]] !=esdata._esi_rs[i] ) cout<<i<<endl;
        
        // till now. SNPs in the three dataset are aligned in bfile order. Alleles are checked by eQTL data as the reference
        // in sparse besd, rowid may be out-of-order
        // bdata._include is the SNPs id to test in case of MAF pruning and HWE pruning being conducted.
        // use bdata._include to extract genotype data for LD
        int outCount = -1;
        unsigned int probNum = esdata._probNum;
        // vectors for output
        vector<long> out_probid(probNum);
        vector<string> bxy(probNum); // origin is double
        vector<string> sexy(probNum);// origin is double
        vector<string> pxy(probNum); // origin is double
        vector<string> bgwas(probNum); // origin is double
        vector<string> beqtl(probNum); // origin is double
        vector<string> segwas(probNum); // origin is double
        vector<string> seeqtl(probNum); // origin is double
        vector<string> pgwas(probNum); // origin is double
        vector<string> peqtl(probNum); // origin is double
        vector<string> rsid(probNum);
        vector<string> rschr(probNum);
        vector<char> rsa1(probNum);
        vector<char> rsa2(probNum);
        vector<string> rsbp(probNum); //origin is unsigned int
        vector<string> rsfreq(probNum); //origin is unsigned double
        vector<string> prb1(probNum); // origin is double
        vector<string> nsnp_test1(probNum);
        vector<string> top_match1(probNum);  // origin is int
        vector<string> ldrsq(probNum); // origin is double
        
        
        
        cout<<endl<<"Performing SMR and heterogeneity analysis..... "<<endl;
        float progr0=0.0 , progr1;
        progress_print(progr0);
        
        
        vector<double> bxz;
		vector<double> sexz;
        vector<uint32_t> curId;
        vector<string> eName;
        vector<int> snpchrom;
        
        vector<char> allele1;
        vector<char> allele2;
        vector<uint32_t> bpsnp;
        vector<double> freq;
        
		vector<double> byz;
		vector<double> seyz;
        VectorXd zsxz;
        
        vector<int> sn_ids;
        
        VectorXd _byz;
        VectorXd _seyz;
        VectorXd _bxz;
        VectorXd _sexz;
        VectorXd _zsxz;
        MatrixXd _X;
        MatrixXd _LD;
        VectorXd ld_v;
        MatrixXd _LD_heidi;
        MatrixXd _X_heidi;
        cis_itvl=cis_itvl*1000;
        
        //for plot
        vector<string> plot_paths;
        for(int i=0;i<probNum;i++)
        {
         
            progr1=1.0*i/probNum;
            if(progr1-progr0-0.05>1e-6 || i+1==probNum)
            {
                if(i+1==probNum) progr1=1.0;
                progress_print(progr1);
                progr0=progr1;
            }
            // for plot
            vector<uint32_t> plot_snpidx;
            vector<uint32_t> plot_probeidx;
            vector<double> plot_bxz;
            vector<double> plot_sexz;
            int plotdir_id=i>>10;
            string plotdir="";
             if(plotflg && !heidioffFlag){
                 for(long j=strlen(outFileName)-1;j>=0;j--)
                     if(outFileName[j]=='/')
                     {
                         plotdir=string(outFileName).substr(0,j+1);
                         break;
                     }
                 if(plotdir=="") plotdir="./";
                 
                 plotdir=string(plotdir)+"plot";
                 struct stat st = {0};
                 if (stat(plotdir.c_str(), &st) == -1) {
                     mkdir(plotdir.c_str(), 0755);
                 }
                 
                 plotdir=string(plotdir)+"/"+atos(plotdir_id);
                 st = {0};
                 if (stat(plotdir.c_str(), &st) == -1) {
                     mkdir(plotdir.c_str(), 0755);
                 }
                 
                 plot_paths.push_back(string(plotdir)+"/"+esdata._epi_prbID[i]);
             }
            //extract info from eqtl summary and gwas summary
            bxz.clear();
            sexz.clear();
            curId.clear(); // is the idxes of bfile._include not the values of
            eName.clear();
            snpchrom.clear();
            byz.clear();
            seyz.clear();
            allele1.clear();
            allele2.clear();
            bpsnp.clear();
            freq.clear();
            long maxid =-9;
            int probebp=esdata._epi_bp[i];
            int probechr=esdata._epi_chr[i];            
            if(esdata._rowid.empty())
            {
                for (int j = 0; j<bdata._include.size(); j++)// bdata._include.size() == esdata._esi_include.size() == gdata._include.size()
                {
                    if (abs(esdata._bxz[i][j] + 9) > 1e-6)
                    {
                        int snpbp=esdata._esi_bp[j];
                        int snpchr=esdata._esi_chr[j];
                        if(snpchr==probechr && ABS(probebp-snpbp)<=cis_itvl)
                        {
                            bxz.push_back(esdata._bxz[i][j]);
                            sexz.push_back(esdata._sexz[i][j]);
                            byz.push_back(gdata.byz[j]);
                            seyz.push_back(gdata.seyz[j]);
                            curId.push_back(j);
                            eName.push_back(esdata._esi_rs[j]);
                            snpchrom.push_back(esdata._esi_chr[j]);
                            if(heidiFlag && esdata._esi_rs[j]==string(refSNP)) maxid=(eName.size()-1);
                            if(!heidioffFlag) //if heidi off , bfile is not necessary to read.
                            {
                                freq.push_back(bdata._mu[bdata._include[j]]/2);
                            }
                            allele1.push_back(esdata._esi_allele1[j]);
                            allele2.push_back(esdata._esi_allele2[j]);
                            bpsnp.push_back(esdata._esi_bp[j]);
                        }
                    }
                }
                
            }
            else{
                uint64_t beta_start=esdata._cols[i<<1];
                uint64_t se_start=esdata._cols[1+(i<<1)];
                uint64_t numsnps=se_start-beta_start;
                for(int j=0;j<numsnps;j++)
                {
                    int ge_rowid=esdata._rowid[beta_start+j];
                    int snpbp=esdata._esi_bp[ge_rowid];
                    int snpchr=esdata._esi_chr[ge_rowid];
                    
                    if(snpchr==probechr && ABS(probebp-snpbp)<=cis_itvl)
                    {
                        bxz.push_back(esdata._val[beta_start+j]);
                        sexz.push_back(esdata._val[se_start+j]);
                        byz.push_back(gdata.byz[ge_rowid]);
                        seyz.push_back(gdata.seyz[ge_rowid]);
                        curId.push_back(ge_rowid);
                        eName.push_back(esdata._esi_rs[ge_rowid]);
                        snpchrom.push_back(esdata._esi_chr[ge_rowid]);
                        if(heidiFlag && esdata._esi_rs[ge_rowid]==string(refSNP)) maxid=(eName.size()-1);
                        allele1.push_back(esdata._esi_allele1[ge_rowid]);
                        allele2.push_back(esdata._esi_allele2[ge_rowid]);
                        bpsnp.push_back(esdata._esi_bp[ge_rowid]);
                        if(!heidioffFlag){
                            freq.push_back(bdata._mu[bdata._include[ge_rowid]]/2);
                        }
                    }
                }
            }
            if(heidiFlag && maxid==-9) continue; //heidi SNP is not in selected SNPs
            if (bxz.size() == 0) continue;
           
            Map<VectorXd> ei_bxz(&bxz[0],bxz.size());
            Map<VectorXd> ei_sexz(&sexz[0],sexz.size());
            
            zsxz=ei_bxz.array()/ei_sexz.array();
            if(!heidiFlag) maxid=max_abs_id(zsxz);
            double pxz_val = pchisq(zsxz[maxid]*zsxz[maxid], 1);
            
            
            if(!heidiFlag && pxz_val>p_smr) continue;
            else outCount++;
            
			double bxy_val = byz[maxid] / bxz[maxid];
			double sexy_val = sqrt((seyz[maxid] * seyz[maxid] * bxz[maxid] * bxz[maxid] + sexz[maxid] * sexz[maxid] * byz[maxid] * byz[maxid]) / (bxz[maxid] * bxz[maxid] * bxz[maxid] * bxz[maxid]));
			double chisqxy = bxy_val*bxy_val / (sexy_val*sexy_val);
            
            
			double pxy_val = pchisq(chisqxy, 1);  //   double pxy=chi_prob(1,chisqxy); //works
            
			double chisqyz = byz[maxid] / seyz[maxid];
			double pyz_val = pchisq(chisqyz*chisqyz, 1);

            out_probid[outCount]= i;
            bxy[outCount]=dtosf(bxy_val);
            sexy[outCount]=dtosf(sexy_val);
            pxy[outCount]=dtos(pxy_val);
            bgwas[outCount]=dtosf(byz[maxid]);
            segwas[outCount]=dtosf(seyz[maxid]);
            beqtl[outCount]=dtosf(bxz[maxid]);
            seeqtl[outCount]=dtosf(sexz[maxid]);
            pgwas[outCount]=dtos(pyz_val);
            peqtl[outCount]=dtos(pxz_val);
            rsid[outCount]=eName[maxid];
            rschr[outCount]=atos(snpchrom[maxid]);
            rsbp[outCount]=itos(bpsnp[maxid]);
            if(heidioffFlag)   rsfreq[outCount]=string("NA");
            else rsfreq[outCount]=dtosf(freq[maxid]);
            
            rsa1[outCount]=allele1[maxid];
            rsa2[outCount]=allele2[maxid];
            
            if(heidioffFlag)
            {
                prb1[outCount]= string("NA");
                nsnp_test1[outCount]= string("NA");
            }
            else
            {
                //extract info from reference
                make_XMat(&bdata,curId, _X); //_X: one row one individual, one column one SNP
                //last vesion ref_snpData was used. row of ref_snpData is SNP, column of ref_snpData is individual
                //cor_calc(_LD, _X);
               
                ld_calc_o2m(ld_v,maxid,_X);
                
               //test here
               // for( int kk=0;kk<curId.size();kk++) if(abs(ld_v[kk]-_LD.col(maxid)[kk])>1e-6) cout<<"wrong"<<endl;
               // test end
                
                sn_ids.clear(); //increase order
                if(abs(ld_top-1)<1e-6) get_square_idxes(sn_ids,zsxz,threshold);
                //else get_square_ldpruning_idxes(sn_ids,zsxz,threshold,_LD, maxid,ld_top);
                else get_square_ldpruning_idxes(sn_ids,zsxz,threshold,ld_v, maxid,ld_top);               
               
                if(sn_ids.size() < m_hetero)
                {
                    prb1[outCount]= string("NA");
                    nsnp_test1[outCount]= string("NA");
                    top_match1[outCount]= string("NA");
                    ldrsq[outCount]= string("NA");
                    continue;
                }
                
                if(plotflg){
                    for(int j=0;j<sn_ids.size();j++) {
                        plot_probeidx.push_back(i);
                        plot_snpidx.push_back(curId[sn_ids[j]]);
                        plot_bxz.push_back(bxz[sn_ids[j]]);
                        plot_sexz.push_back(sexz[sn_ids[j]]);
                    }
                    
                    string ldfile = string(plotdir)+"/"+esdata._epi_prbID[i]+".engs";
                    ofstream ldio(ldfile.c_str());
                    if (!ldio) throw ("Error: can not open the file " + ldfile + " to save!");
                    ldio << "ProbeChr" <<'\t' << "ProbeID"  << '\t' << "Probe_bp" << '\t'<< "SNP_Chr"<< '\t'<< "SNP"<< '\t'  << "SNP_bp"<< '\t'<< "A1"<< '\t'<< "A2"<< '\t'<<"b_GWAS"<<'\t'<<"se_GWAS"<<'\t'<<"b_eQTL"<<'\t'<<"se_eQTL"<<'\n';
                    for (int ii = 0;ii <plot_snpidx.size(); ii++) {
                        ldio<<esdata._epi_chr[plot_probeidx[ii]]<<'\t'<<esdata._epi_prbID[plot_probeidx[ii]]<<'\t'<<esdata._epi_bp[plot_probeidx[ii]]<<'\t'<<esdata._esi_chr[plot_snpidx[ii]]<<'\t'<<esdata._esi_rs[plot_snpidx[ii]]<<'\t'<<esdata._esi_bp[plot_snpidx[ii]]<<'\t'<<esdata._esi_allele1[plot_snpidx[ii]]<<'\t'<<esdata._esi_allele2[plot_snpidx[ii]]<<'\t'<<gdata.byz[plot_snpidx[ii]]<<'\t'<<gdata.seyz[plot_snpidx[ii]]<<'\t'<<plot_bxz[ii]<<'\t'<<plot_sexz[ii]<<'\n';
                    }
                    cout<<"informantion of "<<plot_snpidx.size()<<" probes for plot have been saved in the file [" + ldfile + "]."<<endl;
                    ldio.close();

                }
                _byz.resize(sn_ids.size());
                _seyz.resize(sn_ids.size());
                _bxz.resize(sn_ids.size());
                _sexz.resize(sn_ids.size());
                _zsxz.resize(sn_ids.size());
               // _LD_heidi.resize(sn_ids.size(),sn_ids.size());
                _X_heidi.resize(_X.rows(), sn_ids.size());
                
                #pragma omp parallel for
                for(int j=0;j<sn_ids.size();j++)
                {
                    _byz[j]=byz[sn_ids[j]];
                    _seyz[j]=seyz[sn_ids[j]];
                    _bxz[j]=bxz[sn_ids[j]];
                    _sexz[j]=sexz[sn_ids[j]];
                    _zsxz[j]=zsxz[sn_ids[j]];
               //   for(int k=0;k<=j;k++)_LD_heidi(j,k)=_LD_heidi(k,j)=_LD(sn_ids[j],sn_ids[k]);
                    _X_heidi.col(j)=_X.col(sn_ids[j]);
                }
                _X.resize(0,0);
                cor_calc(_LD_heidi, _X_heidi);
                
                if(plotflg)
                {
                    string ldidfile = string(plotdir)+"/"+esdata._epi_prbID[i]+".ld.id";
                    ofstream ldio(ldidfile.c_str());
                    if (!ldio) throw ("Error: can not open the file " + ldidfile + " to save!");
                    for (int ii = 0;ii <plot_snpidx.size(); ii++)
                        ldio<<esdata._esi_rs[plot_snpidx[ii]]<<'\n';
                    cout<<"rs id of "<<plot_snpidx.size()<<" unique SNPs for plot have been saved in the file [" + ldidfile + "]."<<endl;
                    ldio.close();
                    
                    string ldinfofile = string(plotdir)+"/"+esdata._epi_prbID[i]+".ld.gz";
                    gzFile gz_outfile = gzopen(ldinfofile.c_str(), "wb");
                    string ldstrs="";
                    for(int ii=0;ii<_LD_heidi.cols();ii++)
                        for(int jj=0;jj<ii;jj++) ldstrs+=atos(_LD_heidi(ii,jj))+"\n";
                    
                    if(gzputs(gz_outfile, ldstrs.c_str()) == -1) cout<<"error"<<endl;
                    gzclose(gz_outfile);
                    cout<< "Lower triangle of LD score matrix of "<<plot_snpidx.size()<<" SNPs have been saved by row-major in the file ["+ ldinfofile + "]."<<endl;
                }
                
                _X_heidi.resize(0,0);
                
                long nsnp = sn_ids.size();
                double pdev=bxy_hetero3(_byz,  _bxz, _seyz, _sexz, _zsxz, _LD_heidi, &nsnp);
                
                prb1[outCount] = dtos(pdev);
                if(nsnp>0) nsnp_test1[outCount] = itos(nsnp);
                else nsnp_test1[outCount] = string("NA");
                
                
                // top GWAS SNP ?= top eQTL
                int indx1 = max_abs_id(_zsxz);
                _byz = _byz.array() / _seyz.array();
                int indx2 = max_abs_id(_byz);
                
                if(indx1 == indx2) top_match1[outCount] =itos(1);
                else top_match1[outCount] = itos(0);
                double ldrsqVal = _LD_heidi(indx1, indx2) * _LD_heidi(indx1, indx2);            
                ldrsq[outCount]=dtosf(ldrsqVal);
            }
            
        }
        if(plotflg && !heidioffFlag)
        {
            string ldfile = string(outFileName)+".plotpath.txt";
            ofstream ldio(ldfile.c_str());
            if (!ldio) throw ("Error: can not open the file " + ldfile + " to save!");
           
            for (int i = 0;i <plot_paths.size(); i++) {
                ldio<<plot_paths[i]<<'\n';
            }
            cout<<"informantion of "<<plot_paths.size()<<" probes for plot have been saved in the file [" + ldfile + "]."<<endl;
            ldio.close();

        }

        
        /*
        //genesn in R is just probidx here. can refer to probinfo to get probeid, probenm, chr, gene, bp
        if(plotflg && !heidioffFlag)
        {
            string ldfile = string(outFileName)+".engs";
            ofstream ldio(ldfile.c_str());
            if (!ldio) throw ("Error: can not open the file " + ldfile + " to save!");
            ldio << "Chr" <<'\t' << "ProbeID"  << '\t' << "Prob_bp" << '\t'<< "SNP"<< '\t'  << "SNP_bp"<< '\t'<< "A1"<< '\t'<< "A2"<< '\t'<<"b_GWAS"<<'\t'<<"se_GWAS"<<'\t'<<"b_eQTL"<<'\t'<<"se_eQTL"<<'\n';
            for (int i = 0;i <plot_snpidx.size(); i++) {
                ldio<<esdata._epi_chr[plot_probeidx[i]]<<'\t'<<esdata._epi_prbID[plot_probeidx[i]]<<'\t'<<esdata._epi_bp[plot_probeidx[i]]<<'\t'<<esdata._esi_rs[plot_snpidx[i]]<<'\t'<<esdata._esi_bp[plot_snpidx[i]]<<'\t'<<esdata._esi_allele1[plot_snpidx[i]]<<'\t'<<esdata._esi_allele2[plot_snpidx[i]]<<'\t'<<gdata.byz[plot_snpidx[i]]<<'\t'<<gdata.seyz[plot_snpidx[i]]<<'\t'<<plot_bxz[i]<<'\t'<<plot_sexz[i]<<'\n';
            }
            cout<<"informantion of "<<plot_snpidx.size()<<" probes for plot have been saved in the file [" + ldfile + "]."<<endl;
            ldio.close();
            
            getUnique(plot_snpidx);
            make_XMat(&bdata,plot_snpidx, _X);
            cor_calc(_LD, _X);
           
            string ldidfile = string(outFileName)+".ld.id";
            ldio.open(ldidfile.c_str());
            if (!ldio) throw ("Error: can not open the file " + ldidfile + " to save!");
            for (int i = 0;i <plot_snpidx.size(); i++)
                ldio<<esdata._esi_rs[plot_snpidx[i]]<<'\n';
            cout<<"rs id of "<<plot_snpidx.size()<<" unique SNPs for plot have been saved in the file [" + ldidfile + "]."<<endl;
            ldio.close();
            
            string ldinfofile = string(outFileName)+".ld.gz";
            gzFile gz_outfile = gzopen(ldinfofile.c_str(), "wb");
            string ldstrs="";
            for(int i=0;i<_LD.cols();i++)
                for(int j=0;j<i;j++) ldstrs+=atos(_LD(i,j))+"\n";
          
            if(gzputs(gz_outfile, ldstrs.c_str()) == -1) cout<<"error"<<endl;
            gzclose(gz_outfile);
            cout<< "Lower triangle of LD score matrix of "<<plot_snpidx.size()<<" SNPs have been saved by row-major in the file ["+ ldinfofile + "]."<<endl;
        }
         */
        string smrfile = string(outFileName)+".smr";
        ofstream smr(smrfile.c_str());
        if (!smr) throw ("Error: can not open the file " + smrfile + " to save!");
       
            smr << "ProbeID" <<'\t'<< "ProbeChr" <<'\t' << "Gene"  << '\t' << "Probe_bp" << '\t'<< "SNP"<< '\t' << "SNP_Chr"<< '\t' << "SNP_bp"<< '\t' << "A1"<< '\t'<< "A2"<< '\t'<<"Freq"<<'\t'<<"b_GWAS"<<'\t'<<"se_GWAS"<<'\t'<< "p_GWAS" << '\t'<<"b_eQTL"<<'\t'<<"se_eQTL"<<'\t'<< "p_eQTL" << '\t'<< "b_SMR" << '\t'<< "se_SMR"<< '\t' << "p_SMR" << "\t"<< "p_HET"<< "\t" << "nsnp" << '\n';
            
            for (int i = 0;i <=outCount; i++) {
                smr<<esdata._epi_prbID[out_probid[i]]<<'\t'<<esdata._epi_chr[out_probid[i]]<<'\t'<<esdata._epi_gene[out_probid[i]]<<'\t'<<esdata._epi_bp[out_probid[i]]<<'\t'<<rsid[i]<<'\t'<<rschr[i]<<'\t'<<rsbp[i]<<'\t'<<rsa1[i]<<'\t'<<rsa2[i]<<'\t'<<rsfreq[i]<<'\t'<<bgwas[i]<<'\t'<<segwas[i]<<'\t'<<pgwas[i]<<'\t'<<beqtl[i]<<'\t'<<seeqtl[i]<<'\t'<<peqtl[i]<<'\t'<<bxy[i]<<'\t'<<sexy[i]<<'\t'<<pxy[i]<<'\t'<<prb1[i]<<'\t'<<nsnp_test1[i]<<'\n';
            }
             cout<<"SMR and heterogeneity analysis finished.\nSMR and heterogeneity analysis results of "<<outCount+1<<" probes have been saved in the file [" + smrfile + "]."<<endl;
               smr.close();
       
        free_gwas_data( &gdata);
        
    }
    
    // test all the meQTL or eQTL <=threshold.  toooo many tests and toooo slow
    void smr_trans(char* outFileName, char* bFileName,char* gwasFileName, char* eqtlFileName, double maf, char* indilstName, char* snplstName,char* problstName,bool bFlag,double p_hetero,double ld_top,int m_hetero , char* indilst2remove, char* snplst2exclde, char* problst2exclde, double p_trans,char* refSNP, bool heidioffFlag,int trans_itvl,bool plotflg)
    {
        setNbThreads(thread_num);
        
        bInfo bdata;
        gwasData gdata;
        eqtlInfo esdata;
        double threshold= chi_val(1,p_hetero);
        bool heidiFlag=false;
        
        if(!heidioffFlag && bFileName == NULL ) throw("Error: please input Plink file for SMR analysis by the flag --bfile.");
        if(gwasFileName==NULL) throw("Error: please input GWAS summary data for SMR analysis by the flag --gwas-summary.");
        if(eqtlFileName==NULL) throw("Error: please input eQTL summary data for SMR analysis by the flag --eqtl-summary.");
        if(refSNP!=NULL) heidiFlag=true;
        read_gwas_data( &gdata, gwasFileName);
        read_esifile(&esdata, string(eqtlFileName)+".esi");
        if (snplstName != NULL) extract_eqtl_snp(&esdata, snplstName);
        if(snplst2exclde != NULL) exclude_eqtl_snp(&esdata, snplst2exclde); //no switch the place ahead of extract_eqtl_snp()
        if(!heidioffFlag)
        {
            read_famfile(&bdata, string(bFileName)+".fam");
            if(indilstName != NULL) keep_indi(&bdata,indilstName);
            if(indilst2remove != NULL) remove_indi(&bdata, indilst2remove);
            read_bimfile(&bdata, string(bFileName)+".bim");
            if(snplstName != NULL) extract_snp(&bdata, snplstName);
            if(snplst2exclde != NULL) exclude_snp(&bdata, snplst2exclde);
            allele_check(&bdata, &gdata, &esdata);
            read_bedfile(&bdata, string(bFileName)+".bed");
            if (bdata._mu.empty()) calcu_mu(&bdata);
            if (maf > 0)
            {
                filter_snp_maf(&bdata, maf);
                update_geIndx(&bdata, &gdata, &esdata);
            }
            
        }else
        {
            allele_check(&gdata, &esdata);
        }
        
        update_gwas(&gdata);
        cout<<"Reading eQTL summary data..."<<endl;
        read_epifile(&esdata, string(eqtlFileName)+".epi");
        if(problstName != NULL) extract_prob(&esdata, problstName);
        if(problst2exclde != NULL) exclude_prob(&esdata, problst2exclde); //no switch the place ahead of extract_prob()
        if(bFlag) read_besdfile(&esdata, string(eqtlFileName)+".besd");
        else      read_esdfile(&esdata, string(eqtlFileName)+".esd");
       
        int outCount = -1;
        unsigned int probNum = esdata._probNum;
        // vectors for output
        vector<long> out_probid(probNum);
        vector<string> bxy(probNum); // origin is double
        vector<string> sexy(probNum);// origin is double
        vector<string> pxy(probNum); // origin is double
        vector<string> bgwas(probNum); // origin is double
        vector<string> beqtl(probNum); // origin is double
        vector<string> segwas(probNum); // origin is double
        vector<string> seeqtl(probNum); // origin is double
        vector<string> pgwas(probNum); // origin is double
        vector<string> peqtl(probNum); // origin is double
        vector<string> rsid(probNum);
        vector<string> rschr(probNum);
        vector<char> rsa1(probNum);
        vector<char> rsa2(probNum);
        vector<string> rsbp(probNum); //origin is unsigned int
        vector<string> rsfreq(probNum); //origin is unsigned double
        vector<string> prb1(probNum); // origin is double
        vector<string> nsnp_test1(probNum);
        vector<string> top_match1(probNum);  // origin is int
        vector<string> ldrsq(probNum); // origin is double
        
        // for plot
        vector<uint32_t> plot_snpidx;
        vector<uint32_t> plot_probeidx;
        vector<double> plot_bxz;
        vector<double> plot_sexz;
        
        cout<<endl<<"Performing SMR and heterogeneity analysis..... "<<endl;
        float progr0=0.0 , progr1;
        progress_print(progr0);
        
        
        vector<double> bxz;
        vector<double> sexz;
        vector<uint32_t> curId;
        vector<string> eName;
        vector<int> echr;
        
        vector<char> allele1;
        vector<char> allele2;
        vector<uint32_t> bpsnp;
        vector<double> freq;
        
        vector<double> byz;
        vector<double> seyz;
        VectorXd zsxz;
        
        vector<int> sn_ids;
        
        VectorXd _byz;
        VectorXd _seyz;
        VectorXd _bxz;
        VectorXd _sexz;
        VectorXd _zsxz;
        MatrixXd _X;
        MatrixXd _LD;
        VectorXd ld_v;
        MatrixXd _LD_heidi;
        MatrixXd _X_heidi;
        trans_itvl=trans_itvl*1000;
        for(int i=0;i<probNum;i++)
        {
            
            progr1=1.0*i/probNum;
            if(progr1-progr0-0.05>1e-6 || i+1==probNum)
            {
                if(i+1==probNum) progr1=1.0;
                progress_print(progr1);
                progr0=progr1;
            }
            
            //extract info from eqtl summary and gwas summary
            bxz.clear();
            sexz.clear();
            curId.clear(); // is the idxes of bfile._include not the values of
            eName.clear();
            echr.clear();
            byz.clear();
            seyz.clear();
            allele1.clear();
            allele2.clear();
            bpsnp.clear();
            freq.clear();
            long maxid =-9;
            int probebp=esdata._epi_bp[i];
            int probechr=esdata._epi_chr[i];
            //get top trans SNPs
            vector<int> topTransBP;
            vector<int> topTransRowid;
            vector<float> topbeta;
            if(esdata._rowid.empty())
            {
                for (int j = 0; j<bdata._include.size(); j++)// bdata._include.size() == esdata._esi_include.size() == gdata._include.size()
                {
                    if (abs(esdata._bxz[i][j] + 9) > 1e-6)
                    {
                        int snpbp=esdata._esi_bp[j];
                        int snpchr=esdata._esi_chr[j];
                        float beta=esdata._bxz[i][j];
                        float se=esdata._sexz[i][j];
                        if(snpchr!=probechr || (snpchr==probechr && ABS(probebp-snpbp)>5000000))
                        {
                            float zeqtl=beta/se;
                            float pval=pchisq(zeqtl*zeqtl, 1);
                            if(pval<=p_trans)
                            {
                                topTransBP.push_back(snpbp);
                                topTransRowid.push_back(j);
                            }
                            
                        }
                    }
                }
                
            }
            else{
                uint64_t beta_start=esdata._cols[i<<1];
                uint64_t se_start=esdata._cols[1+(i<<1)];
                uint64_t numsnps=se_start-beta_start;
                for(int j=0;j<numsnps;j++)
                {
                    int ge_rowid=esdata._rowid[beta_start+j];
                    int snpbp=esdata._esi_bp[ge_rowid];
                    int snpchr=esdata._esi_chr[ge_rowid];
                    float beta=esdata._val[beta_start+j];
                    float se=esdata._val[se_start+j];

                    if(snpchr!=probechr || (snpchr==probechr && ABS(probebp-snpbp)>5000000))
                    {
                        float zeqtl=beta/se;
                        float pval=pchisq(zeqtl*zeqtl, 1);
                        if(pval<=p_trans)
                        {
                            topTransBP.push_back(snpbp);
                            topTransRowid.push_back(j); // here j is not row id in dense format. it is the value id of current probe
                            topbeta.push_back(beta);
                            
                        }
                        
                    }
                    
                }
            }
           
            for(int j=0;j<topTransBP.size();j++)
            {
                int curbp=topTransBP[j]; // =esdata._esi_bp[esdata._rowid[esdata._cols[i<<1]+topTransRowid[j]]];
                if(esdata._rowid.empty())
                 {
                     
                 }
                 else{
                     uint64_t beta_start=esdata._cols[i<<1];
                     uint64_t se_start=esdata._cols[1+(i<<1)];
                     uint64_t numsnps=se_start-beta_start;
                     for(int k=0;k<numsnps;k++)
                     {
                         int ge_rowid=esdata._rowid[beta_start+k];
                         int snpbp=esdata._esi_bp[ge_rowid];
                         int snpchr=esdata._esi_chr[ge_rowid];
                         if(ABS(curbp-snpbp)<=trans_itvl)
                         {
                              bxz.push_back(esdata._val[beta_start+k]);
                              sexz.push_back(esdata._val[se_start+k]);
                              byz.push_back(gdata.byz[ge_rowid]);
                              seyz.push_back(gdata.seyz[ge_rowid]);
                              curId.push_back(ge_rowid);
                              eName.push_back(esdata._esi_rs[ge_rowid]);
                             echr.push_back(snpchr);
                             if(heidiFlag && esdata._esi_rs[ge_rowid]==string(refSNP)) maxid=(eName.size()-1);
                             else if(curbp==snpbp) maxid=(eName.size()-1);
                             allele1.push_back(esdata._esi_allele1[ge_rowid]);
                             allele2.push_back(esdata._esi_allele2[ge_rowid]);
                             bpsnp.push_back(esdata._esi_bp[ge_rowid]);
                             if(!heidioffFlag){
                                 freq.push_back(bdata._mu[bdata._include[ge_rowid]]/2);
                             }

                         }
                     }
                 }
                if(heidiFlag && maxid==-9) continue; //heidi SNP is not in selected SNPs
                if (bxz.size() == 0) continue;
                
                Map<VectorXd> ei_bxz(&bxz[0],bxz.size());
                Map<VectorXd> ei_sexz(&sexz[0],sexz.size());
                zsxz=ei_bxz.array()/ei_sexz.array();
                double pxz_val = pchisq(zsxz[maxid]*zsxz[maxid], 1);
               
                double bxy_val = byz[maxid] / bxz[maxid];
                double sexy_val = sqrt((seyz[maxid] * seyz[maxid] * bxz[maxid] * bxz[maxid] + sexz[maxid] * sexz[maxid] * byz[maxid] * byz[maxid]) / (bxz[maxid] * bxz[maxid] * bxz[maxid] * bxz[maxid]));
                double chisqxy = bxy_val*bxy_val / (sexy_val*sexy_val);
                
                double pxy_val = pchisq(chisqxy, 1);  //   double pxy=chi_prob(1,chisqxy); //works
                
                double chisqyz = byz[maxid] / seyz[maxid];
                double pyz_val = pchisq(chisqyz*chisqyz, 1);
                
                outCount++;
                out_probid[outCount]= i;
                bxy[outCount]=dtosf(bxy_val);
                sexy[outCount]=dtosf(sexy_val);
                pxy[outCount]=dtos(pxy_val);
                bgwas[outCount]=dtosf(byz[maxid]);
                segwas[outCount]=dtosf(seyz[maxid]);
                beqtl[outCount]=dtosf(bxz[maxid]);
                seeqtl[outCount]=dtosf(sexz[maxid]);
                pgwas[outCount]=dtos(pyz_val);
                peqtl[outCount]=dtos(pxz_val);
                rsid[outCount]=eName[maxid];
                rschr[outCount]=atos(echr[maxid]);
                rsbp[outCount]=itos(bpsnp[maxid]);
                if(!heidioffFlag){
                    rsfreq[outCount]=dtosf(freq[maxid]);
                }
                rsa1[outCount]=allele1[maxid];
                rsa2[outCount]=allele2[maxid];
                
                if(heidioffFlag)
                {
                    prb1[outCount]= string("NA");
                    nsnp_test1[outCount]= string("NA");

                }
                else
                {
                    make_XMat(&bdata,curId, _X);
                    ld_calc_o2m(ld_v,maxid,_X);
                    
                    sn_ids.clear(); //increase order
                    if(abs(ld_top-1)<1e-6) get_square_idxes(sn_ids,zsxz,threshold);
                    else get_square_ldpruning_idxes(sn_ids,zsxz,threshold,ld_v, maxid,ld_top);
                    
                    if(sn_ids.size() < m_hetero)
                    {
                        prb1[outCount]= string("NA");
                        nsnp_test1[outCount]= string("NA");
                        top_match1[outCount]= string("NA");
                        ldrsq[outCount]= string("NA");
                        continue;
                    }
                    
                    if(plotflg){
                        for(int j=0;j<sn_ids.size();j++) {
                            plot_probeidx.push_back(i);
                            plot_snpidx.push_back(curId[sn_ids[j]]);
                            plot_bxz.push_back(bxz[sn_ids[j]]);
                            plot_sexz.push_back(sexz[sn_ids[j]]);
                        }
                    }
                    _byz.resize(sn_ids.size());
                    _seyz.resize(sn_ids.size());
                    _bxz.resize(sn_ids.size());
                    _sexz.resize(sn_ids.size());
                    _zsxz.resize(sn_ids.size());
                    _X_heidi.resize(_X.rows(), sn_ids.size());
                    
                    #pragma omp parallel for
                    for(int j=0;j<sn_ids.size();j++)
                    {
                        _byz[j]=byz[sn_ids[j]];
                        _seyz[j]=seyz[sn_ids[j]];
                        _bxz[j]=bxz[sn_ids[j]];
                        _sexz[j]=sexz[sn_ids[j]];
                        _zsxz[j]=zsxz[sn_ids[j]];
                        _X_heidi.col(j)=_X.col(sn_ids[j]);
                    }
                    _X.resize(0,0);
                    cor_calc(_LD_heidi, _X_heidi);
                    
                    _X_heidi.resize(0,0);
                    
                    long nsnp = sn_ids.size();
                    double pdev=bxy_hetero3(_byz,  _bxz, _seyz, _sexz, _zsxz, _LD_heidi, &nsnp);
                    
                    prb1[outCount] = dtos(pdev);
                    if(nsnp>0) nsnp_test1[outCount] = itos(nsnp);
                    else nsnp_test1[outCount] = string("NA");
                    
                    
                    // top GWAS SNP ?= top eQTL
                    int indx1 = max_abs_id(_zsxz);
                    _byz = _byz.array() / _seyz.array();
                    int indx2 = max_abs_id(_byz);
                    
                    if(indx1 == indx2) top_match1[outCount] =itos(1);
                    else top_match1[outCount] = itos(0);
                    double ldrsqVal = _LD_heidi(indx1, indx2) * _LD_heidi(indx1, indx2);
                    ldrsq[outCount]=dtosf(ldrsqVal);
                }
                
            }
            
        }
        
        //genesn in R is just probidx here. can refer to probinfo to get probeid, probenm, chr, gene, bp
        if(plotflg && !heidioffFlag)
        {
            string ldfile = string(outFileName)+".engs";
            ofstream ldio(ldfile.c_str());
            if (!ldio) throw ("Error: can not open the file " + ldfile + " to save!");
            ldio << "Chr" <<'\t' << "ProbeID"  << '\t' << "Prob_bp" << '\t'<< "SNP"<< '\t'  << "SNP_bp"<< '\t'<< "A1"<< '\t'<< "A2"<< '\t'<<"b_GWAS"<<'\t'<<"se_GWAS"<<'\t'<<"b_eQTL"<<'\t'<<"se_eQTL"<<'\n';
            for (int i = 0;i <plot_snpidx.size(); i++) {
                ldio<<esdata._epi_chr[plot_probeidx[i]]<<'\t'<<esdata._epi_prbID[plot_probeidx[i]]<<'\t'<<esdata._epi_bp[plot_probeidx[i]]<<'\t'<<esdata._esi_rs[plot_snpidx[i]]<<'\t'<<esdata._esi_bp[plot_snpidx[i]]<<'\t'<<esdata._esi_allele1[plot_snpidx[i]]<<'\t'<<esdata._esi_allele2[plot_snpidx[i]]<<'\t'<<gdata.byz[plot_snpidx[i]]<<'\t'<<gdata.seyz[plot_snpidx[i]]<<'\t'<<plot_bxz[i]<<'\t'<<plot_sexz[i]<<'\n';
            }
            cout<<"informantion of "<<plot_snpidx.size()<<" probes for plot have been saved in the file [" + ldfile + "]."<<endl;
            ldio.close();
            
            getUnique(plot_snpidx);
            make_XMat(&bdata,plot_snpidx, _X);
            cor_calc(_LD, _X);
            
            string ldidfile = string(outFileName)+".ld.id";
            ldio.open(ldidfile.c_str());
            if (!ldio) throw ("Error: can not open the file " + ldidfile + " to save!");
            for (int i = 0;i <plot_snpidx.size(); i++)
                ldio<<esdata._esi_rs[plot_snpidx[i]]<<'\n';
            cout<<"rs id of "<<plot_snpidx.size()<<" unique SNPs for plot have been saved in the file [" + ldidfile + "]."<<endl;
            ldio.close();
            
            string ldinfofile = string(outFileName)+".ld.gz";
            gzFile gz_outfile = gzopen(ldinfofile.c_str(), "wb");
            string ldstrs="";
            for(int i=0;i<_LD.cols();i++)
                for(int j=0;j<i;j++) ldstrs+=atos(_LD(i,j))+"\n";
            
            if(gzputs(gz_outfile, ldstrs.c_str()) == -1) cout<<"error"<<endl;
            gzclose(gz_outfile);
            cout<< "Lower triangle of LD score matrix of "<<plot_snpidx.size()<<" SNPs have been saved by row-major in the file ["+ ldinfofile + "]."<<endl;
        }
        string smrfile = string(outFileName)+".smr";
        ofstream smr(smrfile.c_str());
        if (!smr) throw ("Error: can not open the file " + smrfile + " to save!");
        
            smr << "ProbeID" <<'\t'<< "ProbeChr" <<'\t' << "Gene"  << '\t' << "Prob_bp" << '\t'<< "SNP"<< '\t'<<"SNP_Chr"<<'\t' << "SNP_bp"<< '\t' << "A1"<< '\t'<< "A2"<< '\t'<<"Freq"<<'\t'<<"b_GWAS"<<'\t'<<"se_GWAS"<<'\t'<< "p_GWAS" << '\t'<<"b_eQTL"<<'\t'<<"se_eQTL"<<'\t'<< "p_eQTL" << '\t'<< "b_SMR" << '\t'<< "se_SMR"<< '\t' << "p_SMR" << "\t"<< "p_HET"<< "\t" << "nsnp" << '\n';
            
            for (int i = 0;i <=outCount; i++) {
                smr<<esdata._epi_prbID[out_probid[i]]<<'\t'<<esdata._epi_chr[out_probid[i]]<<'\t'<<esdata._epi_gene[out_probid[i]]<<'\t'<<esdata._epi_bp[out_probid[i]]<<'\t'<<rsid[i]<<'\t'<<rschr[i]<<'\t'<<rsbp[i]<<'\t'<<rsa1[i]<<'\t'<<rsa2[i]<<'\t'<<rsfreq[i]<<'\t'<<bgwas[i]<<'\t'<<segwas[i]<<'\t'<<pgwas[i]<<'\t'<<beqtl[i]<<'\t'<<seeqtl[i]<<'\t'<<peqtl[i]<<'\t'<<bxy[i]<<'\t'<<sexy[i]<<'\t'<<pxy[i]<<'\t'<<prb1[i]<<'\t'<<nsnp_test1[i]<<'\n';
            }
            cout<<"SMR and heterogeneity analysis for trans regions finished.\nSMR and heterogeneity analysis results of "<<outCount+1<<" probes have been saved in the file [" + smrfile + "]."<<endl;
       
        smr.close();
        
        free_gwas_data( &gdata);
        

    }
      
    void smr_trans_wholeInOne(char* outFileName, char* bFileName,char* gwasFileName, char* eqtlFileName, double maf, char* indilstName, char* snplstName,char* problstName,bool bFlag,double p_hetero,double ld_top,int m_hetero , char* indilst2remove, char* snplst2exclde, char* problst2exclde, double p_trans,char* refSNP, bool heidioffFlag,int trans_itvl,bool plotflg)
    {
        setNbThreads(thread_num);
        
        bInfo bdata;
        gwasData gdata;
        eqtlInfo esdata;
        double threshold= chi_val(1,p_hetero);
        bool heidiFlag=false;
        
        if(!heidioffFlag && bFileName == NULL ) throw("Error: please input Plink file for SMR analysis by the flag --bfile.");
        if(gwasFileName==NULL) throw("Error: please input GWAS summary data for SMR analysis by the flag --gwas-summary.");
        if(eqtlFileName==NULL) throw("Error: please input eQTL summary data for SMR analysis by the flag --eqtl-summary.");
        if(refSNP!=NULL) heidiFlag=true;
        read_gwas_data( &gdata, gwasFileName);
        read_esifile(&esdata, string(eqtlFileName)+".esi");
        if (snplstName != NULL) extract_eqtl_snp(&esdata, snplstName);
        if(snplst2exclde != NULL) exclude_eqtl_snp(&esdata, snplst2exclde); //no switch the place ahead of extract_eqtl_snp()
        if(!heidioffFlag)
        {
            read_famfile(&bdata, string(bFileName)+".fam");
            if(indilstName != NULL) keep_indi(&bdata,indilstName);
            if(indilst2remove != NULL) remove_indi(&bdata, indilst2remove);
            read_bimfile(&bdata, string(bFileName)+".bim");
            if(snplstName != NULL) extract_snp(&bdata, snplstName);
            if(snplst2exclde != NULL) exclude_snp(&bdata, snplst2exclde);
            allele_check(&bdata, &gdata, &esdata);
            read_bedfile(&bdata, string(bFileName)+".bed");
            if (bdata._mu.empty()) calcu_mu(&bdata);
            if (maf > 0)
            {
                filter_snp_maf(&bdata, maf);
                update_geIndx(&bdata, &gdata, &esdata);
            }
            
        }else
        {
            allele_check(&gdata, &esdata);
        }
        
        update_gwas(&gdata);
        cout<<"Reading eQTL summary data..."<<endl;
        read_epifile(&esdata, string(eqtlFileName)+".epi");
        if(problstName != NULL) extract_prob(&esdata, problstName);
        if(problst2exclde != NULL) exclude_prob(&esdata, problst2exclde); //no switch the place ahead of extract_prob()
       
        if(bFlag) read_besdfile(&esdata, string(eqtlFileName)+".besd");
        else      read_esdfile(&esdata, string(eqtlFileName)+".esd");
        
        int outCount = -1;
        unsigned int probNum = esdata._probNum;
        // vectors for output
        vector<long> out_probid(probNum);
        vector<string> bxy(probNum); // origin is double
        vector<string> sexy(probNum);// origin is double
        vector<string> pxy(probNum); // origin is double
        vector<string> bgwas(probNum); // origin is double
        vector<string> beqtl(probNum); // origin is double
        vector<string> segwas(probNum); // origin is double
        vector<string> seeqtl(probNum); // origin is double
        vector<string> pgwas(probNum); // origin is double
        vector<string> peqtl(probNum); // origin is double
        vector<string> rsid(probNum);
        vector<string> rschr(probNum);
        vector<char> rsa1(probNum);
        vector<char> rsa2(probNum);
        vector<string> rsbp(probNum); //origin is unsigned int
        vector<string> rsfreq(probNum); //origin is unsigned double
        vector<string> prb1(probNum); // origin is double
        vector<string> nsnp_test1(probNum);
        vector<string> top_match1(probNum);  // origin is int
        vector<string> ldrsq(probNum); // origin is double
        
        // for plot
        vector<uint32_t> plot_snpidx;
        vector<uint32_t> plot_probeidx;
        vector<double> plot_bxz;
        vector<double> plot_sexz;
        
        cout<<endl<<"Performing SMR and heterogeneity analysis..... "<<endl;
        float progr0=0.0 , progr1;
        progress_print(progr0);
        
        
        vector<double> bxz;
        vector<double> sexz;
        vector<uint32_t> curId;
        vector<string> eName;
        vector<int> snpchrom;
        
        vector<char> allele1;
        vector<char> allele2;
        vector<int> bpsnp;
        vector<double> freq;
        
        vector<double> byz;
        vector<double> seyz;
        VectorXd zsxz;
        
        vector<int> sn_ids;
        
        VectorXi _bpsnp;
        VectorXd _byz;
        VectorXd _seyz;
        VectorXd _bxz;
        VectorXd _sexz;
        VectorXd _zsxz;
        MatrixXd _X;
        MatrixXd _LD;
        VectorXd ld_v;
        MatrixXd _LD_heidi;
        MatrixXd _X_heidi;
        trans_itvl=trans_itvl*1000;
        for(int i=0;i<probNum;i++)
        {
            
            progr1=1.0*i/probNum;
            if(progr1-progr0-0.05>1e-6 || i+1==probNum)
            {
                if(i+1==probNum) progr1=1.0;
                progress_print(progr1);
                progr0=progr1;
            }
            
            //extract info from eqtl summary and gwas summary
            bxz.clear();
            sexz.clear();
            curId.clear(); // is the idxes of bfile._include not the values of
            eName.clear();
            snpchrom.clear();
            byz.clear();
            seyz.clear();
            allele1.clear();
            allele2.clear();
            bpsnp.clear();
            freq.clear();
            long maxid =-9;
            int probebp=esdata._epi_bp[i];
            int probechr=esdata._epi_chr[i];
            //get top trans SNPs
            vector<float> topbeta;
            if(esdata._rowid.empty())
            {
                for (int j = 0; j<bdata._include.size(); j++)// bdata._include.size() == esdata._esi_include.size() == gdata._include.size()
                {
                    if (abs(esdata._bxz[i][j] + 9) > 1e-6)
                    {
                        int snpbp=esdata._esi_bp[j];
                        int snpchr=esdata._esi_chr[j];
                        float beta=esdata._bxz[i][j];
                        float se=esdata._sexz[i][j];
                        if(snpchr!=probechr || (snpchr==probechr && ABS(probebp-snpbp)>5000000))
                        {
                            float zeqtl=beta/se;
                            float pval=pchisq(zeqtl*zeqtl, 1);
                            if(pval<=p_trans)
                            {
                                bxz.push_back(esdata._bxz[i][j]);
                                sexz.push_back(esdata._sexz[i][j]);
                                byz.push_back(gdata.byz[j]);
                                seyz.push_back(gdata.seyz[j]);
                                curId.push_back(j);
                                eName.push_back(esdata._esi_rs[j]);
                                snpchrom.push_back(esdata._esi_chr[j]);
                                if(heidiFlag && esdata._esi_rs[j]==string(refSNP)) maxid=(eName.size()-1);
                                if(!heidioffFlag) //if heidi off , bfile is not necessary to read.
                                {
                                    freq.push_back(bdata._mu[bdata._include[j]]/2);
                                }
                                allele1.push_back(esdata._esi_allele1[j]);
                                allele2.push_back(esdata._esi_allele2[j]);
                                bpsnp.push_back(esdata._esi_bp[j]);

                            }
                            
                        }
                    }
                }
                
            }
            else
            {
                uint64_t beta_start=esdata._cols[i<<1];
                uint64_t se_start=esdata._cols[1+(i<<1)];
                uint64_t numsnps=se_start-beta_start;
                for(int j=0;j<numsnps;j++)
                {
                    int ge_rowid=esdata._rowid[beta_start+j];
                    int snpbp=esdata._esi_bp[ge_rowid];
                    int snpchr=esdata._esi_chr[ge_rowid];
                    float beta=esdata._val[beta_start+j];
                    float se=esdata._val[se_start+j];
                    
                    if(snpchr!=probechr || (snpchr==probechr && ABS(probebp-snpbp)>5000000))
                    {
                        float zeqtl=beta/se;
                        float pval=pchisq(zeqtl*zeqtl, 1);
                        if(pval<=p_trans)
                        {
                            bxz.push_back(esdata._val[beta_start+j]);
                            sexz.push_back(esdata._val[se_start+j]);
                            byz.push_back(gdata.byz[ge_rowid]);
                            seyz.push_back(gdata.seyz[ge_rowid]);
                            curId.push_back(ge_rowid);
                            eName.push_back(esdata._esi_rs[ge_rowid]);
                            snpchrom.push_back(esdata._esi_chr[ge_rowid]);
                            if(heidiFlag && esdata._esi_rs[ge_rowid]==string(refSNP)) maxid=(eName.size()-1);
                            allele1.push_back(esdata._esi_allele1[ge_rowid]);
                            allele2.push_back(esdata._esi_allele2[ge_rowid]);
                            bpsnp.push_back(esdata._esi_bp[ge_rowid]);
                            if(!heidioffFlag){
                                freq.push_back(bdata._mu[bdata._include[ge_rowid]]/2);
                            }
                            
                        }
                        
                    }
                    
                }
            }
            
       
                
            if(heidiFlag && maxid==-9) continue; //heidi SNP is not in selected SNPs
            if (bxz.size() == 0) continue;
                
            Map<VectorXd> ei_bxz(&bxz[0],bxz.size());
            Map<VectorXd> ei_sexz(&sexz[0],sexz.size());
            zsxz=ei_bxz.array()/ei_sexz.array();
            if(!heidiFlag) maxid=max_abs_id(zsxz);
            double pxz_val = pchisq(zsxz[maxid]*zsxz[maxid], 1);
                
                double bxy_val = byz[maxid] / bxz[maxid];
                double sexy_val = sqrt((seyz[maxid] * seyz[maxid] * bxz[maxid] * bxz[maxid] + sexz[maxid] * sexz[maxid] * byz[maxid] * byz[maxid]) / (bxz[maxid] * bxz[maxid] * bxz[maxid] * bxz[maxid]));
                double chisqxy = bxy_val*bxy_val / (sexy_val*sexy_val);
                
                double pxy_val = pchisq(chisqxy, 1);  //   double pxy=chi_prob(1,chisqxy); //works
                
                double chisqyz = byz[maxid] / seyz[maxid];
                double pyz_val = pchisq(chisqyz*chisqyz, 1);
                
                outCount++;
                out_probid[outCount]= i;
                bxy[outCount]=dtosf(bxy_val);
                sexy[outCount]=dtosf(sexy_val);
                pxy[outCount]=dtos(pxy_val);
                bgwas[outCount]=dtosf(byz[maxid]);
                segwas[outCount]=dtosf(seyz[maxid]);
                beqtl[outCount]=dtosf(bxz[maxid]);
                seeqtl[outCount]=dtosf(sexz[maxid]);
                pgwas[outCount]=dtos(pyz_val);
                peqtl[outCount]=dtos(pxz_val);
                rsid[outCount]=eName[maxid];
                rschr[outCount]=atos(snpchrom[maxid]);
                rsbp[outCount]=itos(bpsnp[maxid]);
                if(!heidioffFlag){
                    rsfreq[outCount]=dtosf(freq[maxid]);
                }
                rsa1[outCount]=allele1[maxid];
                rsa2[outCount]=allele2[maxid];
                
                if(heidioffFlag)
                {
                    prb1[outCount]= string("NA");
                    nsnp_test1[outCount]= string("NA");
                    
                }
                else
                {
                    //distance vector
                    VectorXd vdistance(curId.size());
                    for(int j=0;j<curId.size();j++)
                        if(ABS(bpsnp[maxid]-bpsnp[j])>1e7) vdistance(j)=0;
                        else vdistance(j)=1;
                    //                   
                    
                    make_XMat(&bdata,curId, _X);
                    ld_calc_o2m(ld_v,maxid,_X);
                   
                    ld_v=ld_v.array()*vdistance.array();
                    
                    sn_ids.clear(); //increase order
                    if(abs(ld_top-1)<1e-6) get_square_idxes(sn_ids,zsxz,threshold);
                    else get_square_ldpruning_idxes(sn_ids,zsxz,threshold,ld_v, maxid,ld_top);
                    
                    if(sn_ids.size() < m_hetero)
                    {
                        prb1[outCount]= string("NA");
                        nsnp_test1[outCount]= string("NA");
                        top_match1[outCount]= string("NA");
                        ldrsq[outCount]= string("NA");
                        continue;
                    }
                    
                    if(plotflg){
                        for(int j=0;j<sn_ids.size();j++) {
                            plot_probeidx.push_back(i);
                            plot_snpidx.push_back(curId[sn_ids[j]]);
                            plot_bxz.push_back(bxz[sn_ids[j]]);
                            plot_sexz.push_back(sexz[sn_ids[j]]);
                        }
                    }
                    _bpsnp.resize(sn_ids.size());
                    _byz.resize(sn_ids.size());
                    _seyz.resize(sn_ids.size());
                    _bxz.resize(sn_ids.size());
                    _sexz.resize(sn_ids.size());
                    _zsxz.resize(sn_ids.size());
                    _X_heidi.resize(_X.rows(), sn_ids.size());
                    
                    #pragma omp parallel for
                    for(int j=0;j<sn_ids.size();j++)
                    {
                        _bpsnp[j]=bpsnp[sn_ids[j]];
                        _byz[j]=byz[sn_ids[j]];
                        _seyz[j]=seyz[sn_ids[j]];
                        _bxz[j]=bxz[sn_ids[j]];
                        _sexz[j]=sexz[sn_ids[j]];
                        _zsxz[j]=zsxz[sn_ids[j]];
                        _X_heidi.col(j)=_X.col(sn_ids[j]);
                    }
                    
                    _X.resize(sn_ids.size(),sn_ids.size()); //as MatrixXd mdist;
                    for(int j=0;j<sn_ids.size();j++)
                        for(int k=0;k<=j;k++)
                            if(ABS(_bpsnp[j]-_bpsnp[k])>1e7) _X(j,k)=_X(k,j)=0;
                            else _X(j,k)=_X(k,j)=1;
                  
                    
                    cor_calc(_LD_heidi, _X_heidi);
               
                    _LD_heidi=_LD_heidi.array()*_X.array();
                    
                    _X_heidi.resize(0,0);
                    _X.resize(0,0); // free space
                    
                    long nsnp = sn_ids.size();
                    double pdev=bxy_hetero3(_byz,  _bxz, _seyz, _sexz, _zsxz, _LD_heidi, &nsnp);
                    
                    prb1[outCount] = dtos(pdev);
                    if(nsnp>0) nsnp_test1[outCount] = itos(nsnp);
                    else nsnp_test1[outCount] = string("NA");
                    
                    
                    // top GWAS SNP ?= top eQTL
                    int indx1 = max_abs_id(_zsxz);
                    _byz = _byz.array() / _seyz.array();
                    int indx2 = max_abs_id(_byz);
                    
                    if(indx1 == indx2) top_match1[outCount] =itos(1);
                    else top_match1[outCount] = itos(0);
                    double ldrsqVal = _LD_heidi(indx1, indx2) * _LD_heidi(indx1, indx2);
                    ldrsq[outCount]=dtosf(ldrsqVal);
                }
                
            }

        
        //genesn in R is just probidx here. can refer to probinfo to get probeid, probenm, chr, gene, bp
        if(plotflg && !heidioffFlag)
        {
            string ldfile = string(outFileName)+".engs";
            ofstream ldio(ldfile.c_str());
            if (!ldio) throw ("Error: can not open the file " + ldfile + " to save!");
            ldio << "Chr" <<'\t' << "ProbeID"  << '\t' << "Prob_bp" << '\t'<< "SNP"<< '\t'  << "SNP_bp"<< '\t'<< "A1"<< '\t'<< "A2"<< '\t'<<"b_GWAS"<<'\t'<<"se_GWAS"<<'\t'<<"b_eQTL"<<'\t'<<"se_eQTL"<<'\n';
            for (int i = 0;i <plot_snpidx.size(); i++) {
                ldio<<esdata._epi_chr[plot_probeidx[i]]<<'\t'<<esdata._epi_prbID[plot_probeidx[i]]<<'\t'<<esdata._epi_bp[plot_probeidx[i]]<<'\t'<<esdata._esi_rs[plot_snpidx[i]]<<'\t'<<esdata._esi_bp[plot_snpidx[i]]<<'\t'<<esdata._esi_allele1[plot_snpidx[i]]<<'\t'<<esdata._esi_allele2[plot_snpidx[i]]<<'\t'<<gdata.byz[plot_snpidx[i]]<<'\t'<<gdata.seyz[plot_snpidx[i]]<<'\t'<<plot_bxz[i]<<'\t'<<plot_sexz[i]<<'\n';
            }
            cout<<"informantion of "<<plot_snpidx.size()<<" probes for plot have been saved in the file [" + ldfile + "]."<<endl;
            ldio.close();
            
            getUnique(plot_snpidx);
            make_XMat(&bdata,plot_snpidx, _X);
            cor_calc(_LD, _X);
            
            string ldidfile = string(outFileName)+".ld.id";
            ldio.open(ldidfile.c_str());
            if (!ldio) throw ("Error: can not open the file " + ldidfile + " to save!");
            for (int i = 0;i <plot_snpidx.size(); i++)
                ldio<<esdata._esi_rs[plot_snpidx[i]]<<'\n';
            cout<<"rs id of "<<plot_snpidx.size()<<" unique SNPs for plot have been saved in the file [" + ldidfile + "]."<<endl;
            ldio.close();
            
            string ldinfofile = string(outFileName)+".ld.gz";
            gzFile gz_outfile = gzopen(ldinfofile.c_str(), "wb");
            string ldstrs="";
            for(int i=0;i<_LD.cols();i++)
                for(int j=0;j<i;j++) ldstrs+=atos(_LD(i,j))+"\n";
            
            if(gzputs(gz_outfile, ldstrs.c_str()) == -1) cout<<"error"<<endl;
            gzclose(gz_outfile);
            cout<< "Lower triangle of LD score matrix of "<<plot_snpidx.size()<<" SNPs have been saved by row-major in the file ["+ ldinfofile + "]."<<endl;
        }
        string smrfile = string(outFileName)+".smr";
        ofstream smr(smrfile.c_str());
        if (!smr) throw ("Error: can not open the file " + smrfile + " to save!");
        
        smr << "ProbeID" <<'\t'<< "ProbeChr" <<'\t' << "Gene"  << '\t' << "Prob_bp" << '\t'<< "SNP"<< '\t'<<"SNP_Chr"<<'\t' << "SNP_bp"<< '\t' << "A1"<< '\t'<< "A2"<< '\t'<<"Freq"<<'\t'<<"b_GWAS"<<'\t'<<"se_GWAS"<<'\t'<< "p_GWAS" << '\t'<<"b_eQTL"<<'\t'<<"se_eQTL"<<'\t'<< "p_eQTL" << '\t'<< "b_SMR" << '\t'<< "se_SMR"<< '\t' << "p_SMR" << "\t"<< "p_HET"<< "\t" << "nsnp" << '\n';
        
        for (int i = 0;i <=outCount; i++) {
            smr<<esdata._epi_prbID[out_probid[i]]<<'\t'<<esdata._epi_chr[out_probid[i]]<<'\t'<<esdata._epi_gene[out_probid[i]]<<'\t'<<esdata._epi_bp[out_probid[i]]<<'\t'<<rsid[i]<<'\t'<<rschr[i]<<'\t'<<rsbp[i]<<'\t'<<rsa1[i]<<'\t'<<rsa2[i]<<'\t'<<rsfreq[i]<<'\t'<<bgwas[i]<<'\t'<<segwas[i]<<'\t'<<pgwas[i]<<'\t'<<beqtl[i]<<'\t'<<seeqtl[i]<<'\t'<<peqtl[i]<<'\t'<<bxy[i]<<'\t'<<sexy[i]<<'\t'<<pxy[i]<<'\t'<<prb1[i]<<'\t'<<nsnp_test1[i]<<'\n';
        }
        cout<<"SMR and heterogeneity analysis for trans regions finished.\nSMR and heterogeneity analysis results of "<<outCount+1<<" probes have been saved in the file [" + smrfile + "]."<<endl;
        
        smr.close();
        
        free_gwas_data( &gdata);
        
        
    }
    

    
    void make_esd_file(char* outFileName, char* bFileName,char* gwasFileName, char* eqtlFileName, double maf, char* indilstName, char* snplstName,char* problstName,bool bFlag,bool make_besd_flag,bool make_esd_flag, char* indilst2remove, char* snplst2exclde, char* problst2exclde, bool cis_flag, int cis_itvl, int trans_itvl, float transThres, float restThres)
    {
        bInfo bdata;
        gwasData gdata;
        eqtlInfo eqtlinfo;
        
        if(bFileName != NULL)
        {
            read_famfile(&bdata, string(bFileName)+".fam");
            if(indilstName != NULL) keep_indi(&bdata,indilstName);
            if(indilst2remove != NULL) remove_indi(&bdata, indilst2remove);
            read_bimfile(&bdata, string(bFileName)+".bim");
            if(snplstName != NULL) extract_snp(&bdata, snplstName);
            if(snplst2exclde != NULL) exclude_snp(&bdata, snplst2exclde);
            read_bedfile(&bdata, string(bFileName)+".bed");
            if (bdata._mu.empty()) calcu_mu(&bdata);
            if(maf>0) filter_snp_maf(&bdata,maf);
            cout<<endl;
        }
        
        if(gwasFileName != NULL) read_gwas_data( &gdata, gwasFileName);
        
        
        cout<<endl<<"Reading eQTL summary data..."<<endl;
        if(eqtlFileName != NULL)
        {
            read_esifile(&eqtlinfo, string(eqtlFileName)+".esi");
            if (snplstName != NULL) extract_eqtl_snp(&eqtlinfo, snplstName);
            if(snplst2exclde != NULL) exclude_eqtl_snp(&eqtlinfo, snplst2exclde); //no switch the place ahead of extract_eqtl_snp()
            read_epifile(&eqtlinfo, string(eqtlFileName)+".epi");
            if(problstName != NULL) extract_prob(&eqtlinfo, problstName);
            if(problst2exclde != NULL) exclude_prob(&eqtlinfo, problst2exclde);
            
            if(bFlag) read_besdfile(&eqtlinfo, string(eqtlFileName)+".besd");
            else      read_esdfile(&eqtlinfo, string(eqtlFileName)+".esd");
            
        }
        else throw ("Error: please input the eQTL summary information for the eQTL data files by the option --beqtl-summary.");
        
        // get common SNPs
        vector<string> slctSNPs;
        vector<char> A1,A2;
        slctSNPs.clear();
        if(bFileName != NULL && gwasFileName != NULL)
        {
            cout<<endl<<"extracting common SNPs among PLink data,GWAS data and eQTL data ..."<<endl;
            vector<int> cmmnId;
            vector<string> cmmnSNPs;
            vector<string> b_snp_name;
            cmmnSNPs.clear();
            cmmnId.clear();
            if(maf>0)
            {
                b_snp_name.resize(bdata._include.size());
                for(int i=0;i<bdata._include.size();i++)
                    b_snp_name[i]=bdata._snp_name[bdata._include[i]];
                StrFunc::match_only(b_snp_name, gdata.snpName, cmmnId);
            }
            else
                StrFunc::match_only(bdata._snp_name, gdata.snpName, cmmnId);
            if(cmmnId.empty()) throw("Error: no common SNPs between PLink data and GWAS data.");
            for(int i=0;i<cmmnId.size();i++) cmmnSNPs.push_back(gdata.snpName[cmmnId[i]]);
            //allele check
            vector<int> bdId;
            StrFunc::match(cmmnSNPs, bdata._snp_name, bdId);
            for (int i = 0; i<cmmnSNPs.size(); i++)
            {
                char a1, a2, ga1, ga2;
                a1 = bdata._allele1[bdId[i]];
                a2 = bdata._allele2[bdId[i]];
                ga1 = gdata.allele_1[cmmnId[i]];
                ga2 = gdata.allele_2[cmmnId[i]];
                if (a1 != '0' && a2 != '0' && ga1 != '0' && ga2 != '0' )
                    if ((a1 == ga1 && a2 == ga2) || (a1 == ga2 && a2 == ga1) )
                    {
                        slctSNPs.push_back(cmmnSNPs[i]);
                        A1.push_back(ga1);
                        A2.push_back(ga2);
                    }
            }
            if(slctSNPs.empty()) throw("Error: no common SNPs between PLink data and GWAS data.");
        }
        else if(bFileName != NULL && gwasFileName == NULL)
        {
            cout<<endl<<"extracting common SNPs between PLink data and eQTL data ..."<<endl;
            
            if(maf>0)
            {
                slctSNPs.resize(bdata._include.size());
                A1.resize(bdata._include.size());
                A2.resize(bdata._include.size());
                for(int i=0;i<bdata._include.size();i++)
                {
                    slctSNPs[i]=bdata._snp_name[bdata._include[i]];
                    A1[i]=bdata._allele1[bdata._include[i]];
                    A2[i]=bdata._allele2[bdata._include[i]];
                }
                
            }
            else
            {
                slctSNPs.assign(bdata._snp_name.begin(),bdata._snp_name.end());
                A1.assign(bdata._allele1.begin(),bdata._allele1.end());
                A2.assign(bdata._allele2.begin(),bdata._allele2.end());
            }
        }
        else if(bFileName == NULL && gwasFileName != NULL)
        {
            cout<<endl<<"extracting common SNPs between GWAS data and eQTL data ..."<<endl;
            slctSNPs.assign(gdata.snpName.begin(),gdata.snpName.end());
            uint32_t length=strlen(gdata.allele_1);
            A1.assign(gdata.allele_1,gdata.allele_1+length);
            A2.assign(gdata.allele_2,gdata.allele_2+length);
        }
        
        if(slctSNPs.empty())
        {
            //output (no Plink data or GWAS data input)
            if(cis_flag)
            {
                cout<<"Transforming dense file to sparse file ..."<<endl;
                if(eqtlinfo._valNum>0) throw("Error: please input dense format eQTL summary data file.");
                //if got flag (--extract-snp), filter_probe_null() should be invoked, then _include.
                vector<uint64_t> cols((eqtlinfo._probNum<<1)+1);
                vector<uint32_t> rowids;
                vector<float> val;
                FILE* logfile=NULL;
     
                //char cCurrentPath[FILENAME_MAX];
                
               // if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
               // {
               //    throw ("Error: can not get current directory!");
               // }
              //  cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';
              //  string fname=string(cCurrentPath)+"/smr.log";
     
                string logfname = string(outFileName)+".summary";
                logfile=fopen(logfname.c_str(), "w");
                if (!(logfile)) {
                    printf("Error: Failed to open log file.\n");
                    exit(1);
                }
                string logstr="--cis-itvl:\t"+itos(cis_itvl)+"Kb\n--trans-itvl:\t"+itos(trans_itvl)+"Kb\n--trans-thres:\t"+dtos(transThres)+"\n--rest-thres:\t"+dtos(restThres)+"\n";
                logstr+="cis region is represent as [Chr, Start bp, End bp, nsnp,trans_touch_cis]; trans region is represent as <Chr, Start bp, End bp, nsnp, extend, merge>.\n";
                logstr+="trans_touch_cis: cis region and trans region are overlapped then merged into one big cis region.\n";
                logstr+="extend: trans region contains more than two SNPs that meet the trans threshold.\n";
                logstr+="merge: two or more trans regions are overlapped and megered into one big trans region.\n\n";
                logstr+="{ProbeID,ProbeBP}\t[Chr,cis_startBP,cis_endBP,NumSNPs,merged]\t<Chr,trans_startBP,trans_endBP,NumSNPs,merged>\tNumSNPs beyond cis and trans\n";
                fputs(logstr.c_str(),logfile);
                fflush(logfile);
                cis_itvl=cis_itvl*1000;
                trans_itvl=trans_itvl*1000;
                cols[0]=0;
                //in case of too many values over transThres in the trans region
                string pretransPrbId="";
                // for log file
                vector<int> tran_chr;
                vector<uint64_t> lowerBp;
                vector<uint64_t> upperBp;
                vector<uint64_t> nsnp;
                vector<bool> extend;
                vector<bool> merge;
                //
                 vector<int> cis_idx;
                for(uint32_t i=0;i<eqtlinfo._probNum;i++)
                {
                    vector<int> esi_include;
                    int probchr=eqtlinfo._epi_chr[i];
                    int probbp= eqtlinfo._epi_bp[i];
                    uint64_t cisuperBounder= probbp+cis_itvl;
                    uint64_t cislowerBounder=((probbp-cis_itvl>0)?(probbp-cis_itvl):0);
                    uint64_t cisNum=0;
                    uint64_t otherSlctNum=0;
                    
                    // for log file
                    tran_chr.clear();
                    lowerBp.clear();
                    upperBp.clear();
                    nsnp.clear();
                    extend.clear();
                    merge.clear();
                    
                    uint64_t realcisUBp=0;
                    uint64_t realcisLBp=0;
                    uint64_t realcisNum=0;
                   
                    bool trans_touch_cis_upper = false;
                    bool trans_touch_cis_lower = false;
                    //
                    cis_idx.clear();
                    for(int j=0;j<eqtlinfo._snpNum;j++)
                    {
                        if(abs(eqtlinfo._sexz[i][j]+9)<1e-6) continue;
                        
                        double zsxz=eqtlinfo._bxz[i][j]/eqtlinfo._sexz[i][j];
                        double pxz=pchisq(zsxz*zsxz, 1);
                        
                        if(eqtlinfo._esi_chr[j] == probchr && eqtlinfo._esi_bp[j]<=cisuperBounder && eqtlinfo._esi_bp[j]>=cislowerBounder && abs(eqtlinfo._sexz[i][j]+9)>1e-6)
                        {
                            cis_idx.push_back(j);
                            esi_include.push_back(j);
                            cisNum++;
                        }
                        else if(pxz<transThres)
                        {
                            uint64_t transNum=0;
                            uint32_t curChr=eqtlinfo._esi_chr[j];
                            esi_include.push_back(j);
                            long transbp=eqtlinfo._esi_bp[j];
                            long translowerBounder=((transbp-trans_itvl>0)?(transbp-trans_itvl):0);
                            long transuperBounder=transbp+trans_itvl;
                            bool extended=false;
                            bool merged=false;
                            
                            // assume esi is sorted
                            int startptr=j-1;
                            while(startptr>=0 && curChr == eqtlinfo._esi_chr[startptr] && transbp-eqtlinfo._esi_bp[startptr]<=trans_itvl)
                            {
                                if(abs(eqtlinfo._sexz[i][startptr]+9)>1e-6)
                                {
                                    translowerBounder=eqtlinfo._esi_bp[startptr];
                                    esi_include.push_back(startptr);
                                    if(upperBp.size()>0 && translowerBounder<=upperBp[upperBp.size()-1]) //trans region merges
                                    {
                                        merged=true;
                                        transNum=transNum+nsnp[nsnp.size()-1];
                                        break;
                                    }
                                    if(eqtlinfo._esi_chr[startptr] == probchr && translowerBounder<=cisuperBounder && translowerBounder>=cislowerBounder) // trans touches cis region
                                    {
                                        trans_touch_cis_upper=true;
                                        transNum=transNum+cis_idx.size();
                                        translowerBounder=eqtlinfo._esi_bp[cis_idx[0]];
                                        break;
                                    }
                                    transNum++;
                                }
                                startptr--;
                            }
                            startptr=j+1;
							while (startptr<eqtlinfo._snpNum && curChr == eqtlinfo._esi_chr[startptr] && eqtlinfo._esi_bp[startptr] - transbp <= trans_itvl)
                            {
                                if(abs(eqtlinfo._sexz[i][startptr]+9)>1e-6)
                                {
                                    transuperBounder=eqtlinfo._esi_bp[startptr];
                                    if(eqtlinfo._esi_chr[startptr] == probchr && transuperBounder>=cislowerBounder && transuperBounder<=cisuperBounder) // trans touches cis region
                                    {
                                        trans_touch_cis_lower=true;
                                        cisNum=transNum;
                                        while(eqtlinfo._esi_chr[startptr] == probchr && eqtlinfo._esi_bp[startptr]<=cisuperBounder && eqtlinfo._esi_bp[startptr]>=cislowerBounder )
                                        {
                                            if(abs(eqtlinfo._sexz[i][startptr]+9)>1e-6)
                                            {
                                                transuperBounder=eqtlinfo._esi_bp[startptr];
                                                esi_include.push_back(startptr);
                                                cisNum++;
                                            }
                                            startptr++;
                                        }
                                        j=startptr; // because cis_itvl >= trans_itvl. so here startptr can't satisfy the outer while loop anymore. j would jump to outer for loop
                                        break; // I think it doesnot matter with or without break here.
                                    }
                                    else
                                    {
                                        esi_include.push_back(startptr);
                                        double zsxz_tmp=eqtlinfo._bxz[i][startptr]/eqtlinfo._sexz[i][startptr];
                                        double pxz_tmp=pchisq(zsxz_tmp*zsxz_tmp, 1);
                                        if(pxz_tmp<transThres) // trans region extends
                                        {
                                            extended=true;
                                            j=startptr;
                                            transbp=eqtlinfo._esi_bp[j];
                                        }
                                        transNum++;
                                        j=startptr;
                                    }
                                }
                                startptr++;
                            }
                            if(!trans_touch_cis_lower && !trans_touch_cis_upper)
                            {
                                if(merged)
                                {
                                    upperBp[upperBp.size()-1]=transuperBounder;
                                    nsnp[nsnp.size()-1]=transNum;
                                    extend[nsnp.size()-1]=extended;
                                    merge[nsnp.size()-1]=merged;
                                }
                                else
                                {
                                    tran_chr.push_back(curChr);
                                    upperBp.push_back(transuperBounder);
                                    lowerBp.push_back(translowerBounder);
                                    nsnp.push_back(transNum);
                                    extend.push_back(extended);
                                    merge.push_back(merged);
                                }

                            }
                            else
                            {
                                realcisNum=transNum;
                                realcisUBp=transuperBounder;
                                realcisLBp=translowerBounder;
                            }
                        }
                        else if(pxz<restThres)
                        {
                            esi_include.push_back(j);
                            otherSlctNum++;
                        }
                    }
                     if(cis_idx.size()>0 && !trans_touch_cis_lower && !trans_touch_cis_upper) // in case of the probe on other chr has no trans or otherSlct on current chr
                     {
                         realcisNum=cis_idx.size();
                         realcisLBp=eqtlinfo._esi_bp[cis_idx[0]];
                         realcisUBp=eqtlinfo._esi_bp[cis_idx[realcisNum-1]];
                     }
                    // log file
                    if(realcisNum || nsnp.size() || otherSlctNum)
                    {
                        logstr="{"+eqtlinfo._epi_prbID[i]+","+atos(eqtlinfo._epi_bp[i])+"}\t";
                        if(realcisNum)
                        {
                            string overlapflg="F";
                            if(trans_touch_cis_lower || trans_touch_cis_upper) overlapflg="T";
                            logstr+= "["+atos(probchr)+","+ atos(realcisLBp)+","+atos(realcisUBp)+","+atos(realcisNum)+","+overlapflg+"]\t";
                        }else logstr+="[]\t";
                        if(nsnp.size())
                        {
                            for(int h=0;h<nsnp.size();h++)
                            {
                                string mergeflg="F";
                                if(extend[h] || merge[h]) mergeflg="T";
                                logstr+= "<"+atos(tran_chr[h])+","+ atos(lowerBp[h])+","+atos(upperBp[h])+","+atos(nsnp[h])+","+mergeflg+">\t";
                            }
                        }else logstr+="<>\t";
                        
                        if(otherSlctNum) logstr+="("+atos(otherSlctNum)+")\n";
                        else logstr+="()\n";
                        
                        fputs(logstr.c_str(),logfile);
                        fflush(logfile);
                    }
                   
                    //
                    sort( esi_include.begin(), esi_include.end() );
                    vector<int> ::iterator it=unique(esi_include.begin(),esi_include.end());
                    esi_include.erase( it, esi_include.end() );
                    
                    for(int j=0;j<esi_include.size();j++)
                    {
                        val.push_back(eqtlinfo._bxz[i][esi_include[j]]);
                        rowids.push_back(esi_include[j]);
                    }
                    for(int j=0;j<esi_include.size();j++)
                    {
                        val.push_back(eqtlinfo._sexz[i][esi_include[j]]);
                        rowids.push_back(esi_include[j]);
                    }
                    uint64_t real_num=esi_include.size();
                    cols[(i<<1)+1]=real_num+cols[i<<1];
                    cols[i+1<<1]=(real_num<<1)+cols[i<<1];
                }
                
                uint64_t colSize=sizeof(uint64_t)*cols.size();
                uint64_t rowSize=sizeof(uint32_t)*rowids.size();
                uint64_t valSize=sizeof(float)*val.size();
                uint64_t valNum=val.size();
                uint64_t bufsize=sizeof(float)+sizeof(uint64_t)+colSize+rowSize+valSize;
                char* buffer=(char*)malloc (bufsize*sizeof(char));
                char* wptr=buffer;
                float ftype=SPARSE_FILE_TYPE_3;
                memcpy(wptr,&ftype,sizeof(float));
                wptr+=sizeof(float);
                memcpy(wptr,&valNum,sizeof(uint64_t));
                wptr+=sizeof(uint64_t);
                memcpy(wptr,&cols[0],colSize);
                wptr+=colSize;
                memcpy(wptr,&rowids[0],rowSize);
                wptr+=rowSize;
                memcpy(wptr,&val[0],valSize);
                
                cout<<"\nsaving eQTL data..."<<endl;
                string esdfile = string(outFileName)+".esi";
                ofstream smr(esdfile.c_str());
                if (!smr) throw ("Error: can not open the esi file " + esdfile + " to save!");
                for (int i = 0;i <eqtlinfo._snpNum; i++) {
                    smr<<eqtlinfo._esi_chr[i]<<'\t'<<eqtlinfo._esi_rs[i]<<'\t'<<eqtlinfo._esi_gd[i]<<'\t'<<eqtlinfo._esi_bp[i]<<'\t'<<eqtlinfo._esi_allele1[i]<<'\t'<<eqtlinfo._esi_allele2[i]<<'\n';
                }
                smr.close();
                cout<<eqtlinfo._snpNum<<" SNPs have been saved in the file [" + esdfile + "]."<<endl;
                
                esdfile = string(outFileName)+".epi";
                smr.open(esdfile.c_str());
                if (!smr) throw ("Error: can not open the epi file " + esdfile + " to save!");
                for (int i = 0;i <eqtlinfo._probNum; i++) {
                    smr<<eqtlinfo._epi_chr[i]<<'\t'<<eqtlinfo._epi_prbID[i]<<'\t'<<eqtlinfo._epi_gd[i]<<'\t'<<eqtlinfo._epi_bp[i]<<'\t'<<eqtlinfo._epi_gene[i]<<'\t'<<eqtlinfo._epi_orien[i]<<'\n';
                }
                smr.close();
                cout<<eqtlinfo._probNum<<" probes have been saved in the file [" + esdfile + "]."<<endl;
                
                esdfile = string(outFileName)+".besd";
                FILE * besd;
                besd = fopen (esdfile.c_str(), "wb");
                fwrite (buffer,sizeof(char), bufsize, besd);
                fclose(besd);
                if(logfile) fclose(logfile);
                free(buffer);
                cout<<"Beta values and SE values for "<<eqtlinfo._include.size()<<" Probes and "<<eqtlinfo._snpNum<<" SNPs have been saved in the binary file [" + esdfile + "]." <<endl;
                cout<<"Log information has been saved in the file [" + logfname + "]." <<endl;
            }
            else
            {
                filter_probe_null(&eqtlinfo); // at the same time, reset the vector _include
                cout<<"\nsaving eQTL data..."<<endl;
                string esdfile = string(outFileName)+".esi";
                ofstream smr(esdfile.c_str());
                if (!smr) throw ("Error: can not open the esi file " + esdfile + " to save!");
                for (int i = 0;i <eqtlinfo._snpNum; i++) {
                    smr<<eqtlinfo._esi_chr[i]<<'\t'<<eqtlinfo._esi_rs[i]<<'\t'<<eqtlinfo._esi_gd[i]<<'\t'<<eqtlinfo._esi_bp[i]<<'\t'<<eqtlinfo._esi_allele1[i]<<'\t'<<eqtlinfo._esi_allele2[i]<<'\n';
                }
                smr.close();
                cout<<eqtlinfo._snpNum<<" SNPs have been saved in the file [" + esdfile + "]."<<endl;
                
                esdfile = string(outFileName)+".epi";
                smr.open(esdfile.c_str());
                if (!smr) throw ("Error: can not open the epi file " + esdfile + " to save!");
                for (int i = 0;i <eqtlinfo._include.size(); i++) {
                    smr<<eqtlinfo._epi_chr[eqtlinfo._include[i]]<<'\t'<<eqtlinfo._epi_prbID[eqtlinfo._include[i]]<<'\t'<<eqtlinfo._epi_gd[eqtlinfo._include[i]]<<'\t'<<eqtlinfo._epi_bp[eqtlinfo._include[i]]<<'\t'<<eqtlinfo._epi_gene[eqtlinfo._include[i]]<<'\t'<<eqtlinfo._epi_orien[eqtlinfo._include[i]]<<'\n';
                }
                smr.close();
                cout<<eqtlinfo._include.size()<<" probes have been saved in the file [" + esdfile + "]."<<endl;
                if(make_esd_flag)
                {
                    esdfile = string(outFileName)+".esd";
                    smr.open(esdfile.c_str());
                    if (!smr) throw ("Error: can not open the esd file " + esdfile + " to save!");
                    if(eqtlinfo._valNum==0)
                    {
                        for (int i = 0;i <eqtlinfo._snpNum; i++) {
                            for(int j=0;j<eqtlinfo._include.size();j++)
                            {
                                double v_tmp=eqtlinfo._sexz[eqtlinfo._include[j]][i];
                                if(abs(v_tmp+9)<1e-6) smr<<"NA"<<'\t'<<"NA"<<'\t';
                                else smr<<eqtlinfo._bxz[eqtlinfo._include[j]][i]<<'\t'<<eqtlinfo._sexz[eqtlinfo._include[j]][i]<<'\t';
                            }
                            smr<<'\n';
                        }
                    }
                    else
                    {   // if output transposed file, should be more easy.
                        eqtlinfo._bxz.resize(eqtlinfo._include.size());
                        eqtlinfo._sexz.resize(eqtlinfo._include.size());
                        for( uint32_t i=0;i<eqtlinfo._include.size();i++)
                        {
                            eqtlinfo._bxz[i].reserve(eqtlinfo._esi_include.size());
                            eqtlinfo._sexz[i].reserve(eqtlinfo._esi_include.size());
                        }
                        for(uint32_t i=0;i<eqtlinfo._include.size();i++)
                            for(uint32_t j=0;j<eqtlinfo._esi_include.size();j++)
                            {
                                eqtlinfo._bxz[i][j]=-9;
                                eqtlinfo._sexz[i][j]=-9;
                            }
                        for(uint32_t i=0;i<eqtlinfo._include.size();i++)
                        {
                            uint64_t proid=eqtlinfo._include[i];
                            uint64_t pos=eqtlinfo._cols[proid<<1];
                            uint64_t pos1=eqtlinfo._cols[(proid<<1)+1];
                            uint64_t num=pos1-pos;
                            for(int j=0;j<num;j++)
                            {
                                eqtlinfo._bxz[i][eqtlinfo._rowid[pos+j]]=eqtlinfo._val[pos+j];
                                eqtlinfo._sexz[i][eqtlinfo._rowid[pos+j]]=eqtlinfo._val[pos+j+num];
                            }
                        }
                        for (uint32_t i = 0;i <eqtlinfo._snpNum; i++) {
                            for(uint32_t j=0;j<eqtlinfo._include.size();j++)
                            {
                                double v_tmp=eqtlinfo._sexz[j][i];
                                if(abs(v_tmp+9)<1e-6) smr<<"NA"<<'\t'<<"NA"<<'\t';
                                else smr<<eqtlinfo._bxz[j][i]<<'\t'<<eqtlinfo._sexz[j][i]<<'\t';
                            }
                            smr<<'\n';
                        }
                    }
                    smr.close();
                    cout<<"Beta values and SE values for "<<eqtlinfo._include.size()<<" Probes and "<<eqtlinfo._snpNum<<" SNPs have been saved in the file [" + esdfile + "]." <<endl;
                    
                 //   esdfile = string(outFileName)+".tesd";
                  //   smr.open(esdfile.c_str());
                  //   if (!smr) throw ("Error: can not open the fam file " + esdfile + " to save!");
                  //   for (int i = 0;i <eqtlinfo._include.size(); i++) {
                  //   for(int j=0;j<eqtlinfo._snpNum;j++)
                 //    {
                  //   double v_tmp=eqtlinfo._sexz[eqtlinfo._include[i]][j];
                  //   if(abs(v_tmp+9)<1e-6) smr<<"NA"<<'\t';
                  //   else smr<<eqtlinfo._bxz[eqtlinfo._include[i]][j]<<'\t';
                 //   }
                  //   smr<<'\n';
                  //   for(int j=0;j<eqtlinfo._snpNum;j++)
                  //   {
                  //   double v_tmp=eqtlinfo._sexz[eqtlinfo._include[i]][j];
                  //   if(abs(v_tmp+9)<1e-6) smr<<"NA"<<'\t';
                  //   else smr<<eqtlinfo._sexz[eqtlinfo._include[i]][j]<<'\t';
                  //   }
                  //   smr<<'\n';
                  //   }
                  //   smr.close();
                  //   cout<<"Beta values and SE values for "<<eqtlinfo._include.size()<<" Probes and "<<eqtlinfo._snpNum<<" SNPs have been saved in the transoped file [" + esdfile + "]." <<endl;
     
                }
                
                if(make_besd_flag)
                {
                    esdfile = string(outFileName)+".besd";
                    FILE * smr;
                    smr = fopen (esdfile.c_str(), "wb");
                    if(eqtlinfo._valNum==0)
                    {
                        uint64_t bsize=(eqtlinfo._include.size()*eqtlinfo._snpNum<<1)+1;
                        float* buffer=(float*)malloc (sizeof(float)*bsize);
                        memset(buffer,0,sizeof(float)*bsize);
                        float* ptr=buffer;
                        *ptr++=0.0;
                        uint64_t pro_num=eqtlinfo._include.size();
                        uint64_t snp_num=eqtlinfo._snpNum;
                        for(int i=0;i<pro_num;i++)
                        {
                            memcpy(ptr+(i<<1)*snp_num,&eqtlinfo._bxz[eqtlinfo._include[i]][0],sizeof(float)*snp_num);
                            memcpy(ptr+((i<<1)+1)*snp_num,&eqtlinfo._sexz[eqtlinfo._include[i]][0],sizeof(float)*snp_num);
                        }
                        fwrite (buffer,sizeof(float), bsize, smr);
                        free(buffer);
                    }
                    else
                    {
                        uint64_t colSize=sizeof(uint64_t)*((eqtlinfo._include.size()<<1)+1);
                        uint64_t rowSize=sizeof(uint32_t)*eqtlinfo._valNum;
                        uint64_t valSize=sizeof(float)*eqtlinfo._valNum;
                        uint64_t valNum=eqtlinfo._valNum;
                        uint64_t bufsize=sizeof(float)+sizeof(uint64_t)+colSize+rowSize+valSize;
                        
                        char* buffer=(char*)malloc (sizeof(char)*bufsize);
                        memset(buffer,0,sizeof(char)*bufsize);
                        float ftype=SPARSE_FILE_TYPE_3;
                        memcpy(buffer,&ftype,sizeof(float));
                        char* wptr=buffer+sizeof(float);
                        memcpy(wptr,&valNum,sizeof(uint64_t));
                        wptr+=sizeof(uint64_t);
                        uint64_t* uptr=(uint64_t*)wptr; *uptr++=0;
                        for(int i=0;i<eqtlinfo._include.size();i++)
                        {
                            *uptr++=eqtlinfo._cols[(eqtlinfo._include[i]<<1)+1];
                            *uptr++=eqtlinfo._cols[eqtlinfo._include[i]+1<<1];
                        }
                        wptr+=colSize;
                        memcpy(wptr,&eqtlinfo._rowid[0],rowSize);
                        wptr+=rowSize;
                        memcpy(wptr,&eqtlinfo._val[0],valSize);
                        fwrite (buffer,sizeof(char), bufsize, smr);
                        free(buffer);
                    }
                    fclose (smr);
                    
                    cout<<"Beta values and SE values for "<<eqtlinfo._include.size()<<" Probes and "<<eqtlinfo._snpNum<<" SNPs have been saved in the binary file [" + esdfile + "]." <<endl;
                }
            }
        }
        else
        {
            vector<int> cmmnId, slctId;
            vector<string> cmmnSNPs;
            cmmnSNPs.clear();
            cmmnId.clear();
            StrFunc::match_only(slctSNPs, eqtlinfo._esi_rs, cmmnId);
            if(cmmnId.empty()) throw("Error: no common SNPs.");
            for(int i=0;i<cmmnId.size();i++) cmmnSNPs.push_back(eqtlinfo._esi_rs[cmmnId[i]]);
            
            vector<int> slId;
            StrFunc::match(cmmnSNPs, slctSNPs, slId);
            slctSNPs.clear();
            for (int i = 0; i<cmmnSNPs.size(); i++)
            {
                char a1, a2, ea1, ea2;
                a1 = A1[slId[i]];
                a2 = A2[slId[i]];
                ea1 = eqtlinfo._esi_allele1[cmmnId[i]];
                ea2 = eqtlinfo._esi_allele2[cmmnId[i]];
                if (a1 != '0' && a2 != '0' && ea1 != '0' && ea2 != '0')
                    if ((a1 == ea1 && a2 == ea2) || (a1 == ea2 && a2 == ea1) || (a1 != ea1 && a1 != ea2 && a2 != ea2 && a2 != ea1))
                    {
                        slctSNPs.push_back(cmmnSNPs[i]);
                        slctId.push_back(cmmnId[i]);  // something like _esi_include
                    }
                
            }
            if(slctSNPs.empty()) throw("Error: no common SNPs after allele check.");
            cout<<slctSNPs.size()<<" SNPs in common to be included after allele check."<<endl;
            
            cout<<"\nfiltering out the probes with no value..."<<endl; // should be invoked always here
            vector<int> include; // something like _include
            if(eqtlinfo._valNum==0)
            {
                for (int i = 0; i < eqtlinfo._probNum; i++)
                {
                    bool NA_flag = true;
                    for (int j = 0; j<slctId.size(); j++)
                    {
                        if (abs(eqtlinfo._sexz[i][slctId[j]] + 9) > 1e-6)
                        {
                            NA_flag = false;
                            break;
                        }
                    }
                    if (!NA_flag) include.push_back(i);
                }
            }
            else
            {
                map<int, int > _incld_id_map;
                for (int i = 0; i<slctId.size(); i++)
                {
                    _incld_id_map.insert(pair<int, int>(slctId[i], i));
                    
                }

                for (int i = 0; i < eqtlinfo._probNum; i++)
                {
                    uint64_t pos1=eqtlinfo._cols[(i<<1)+1];
                    uint64_t pos=eqtlinfo._cols[i<<1];
                    uint64_t num=pos1-pos;
                    if (num>0)
                    {
                        for(int j=0;j<num;j++)
                        {
                            uint32_t rid=eqtlinfo._rowid[pos+j];
                            map<int, int>::iterator iter;
                            iter=_incld_id_map.find(rid);
                            if(iter!=_incld_id_map.end())
                            {
                                include.push_back(i);
                                break;
                            }
                        }
                    }
                }
            }
            
            cout<<include.size()<<" probes to be included."<<endl;
            
            //output
            cout<<"\nsaving eQTL data..."<<endl;
            string esdfile = string(outFileName) + ".esi";
            ofstream smr(esdfile.c_str());
            if (!smr) throw ("Error: can not open the fam file " + esdfile + " to save!");
            for (int i = 0; i <slctId.size(); i++) {
                smr << eqtlinfo._esi_chr[slctId[i]] << '\t' << eqtlinfo._esi_rs[slctId[i]] << '\t' << eqtlinfo._esi_gd[slctId[i]] << '\t' << eqtlinfo._esi_bp[slctId[i]] << '\t' << eqtlinfo._esi_allele1[slctId[i]] << '\t' << eqtlinfo._esi_allele2[slctId[i]] << '\n';
            }
            smr.close();
            cout << slctId.size() << " SNPs have been saved in the file [" + esdfile + "]." << endl;
            
            esdfile = string(outFileName) + ".epi";
            smr.open(esdfile.c_str());
            if (!smr) throw ("Error: can not open the fam file " + esdfile + " to save!");
            for (int i = 0; i <include.size(); i++) {
                smr << eqtlinfo._epi_chr[include[i]] << '\t' << eqtlinfo._epi_prbID[include[i]] << '\t' << eqtlinfo._epi_gd[include[i]] << '\t' << eqtlinfo._epi_bp[include[i]] << '\t' << eqtlinfo._epi_gene[include[i]] << '\t' << eqtlinfo._epi_orien[include[i]] << '\n';
            }
            smr.close();
            cout << include.size() << " probes have been saved in the file [" + esdfile + "]." << endl;
            if(make_esd_flag)
            {
                esdfile = string(outFileName) + ".esd";
                smr.open(esdfile.c_str());
                if (!smr) throw ("Error: can not open the fam file " + esdfile + " to save!");
                if(eqtlinfo._valNum==0)
                {
                    for (int i = 0; i <slctId.size(); i++) {
                        for (int j = 0; j<include.size(); j++)
                        {
                            double v_tmp = eqtlinfo._sexz[include[j]][slctId[i]];
                            if (abs(v_tmp + 9)<1e-6) smr << "NA" << '\t' << "NA" << '\t';
                            else smr << eqtlinfo._bxz[include[j]][slctId[i]] << '\t' << eqtlinfo._sexz[include[j]][slctId[i]] << '\t';
                        }
                        smr << '\n';
                    }
                }
                else
                {
                    vector<int> rk;
                    getRank(slctId, rk);
                    map<int, int > _incld_id_map;
                    for (int i = 0; i<slctId.size(); i++)
                    {
                        _incld_id_map.insert(pair<int, int>(slctId[i], i));
                        
                    }
                    eqtlinfo._bxz.resize(include.size());
                    eqtlinfo._sexz.resize(include.size());
                    for( uint32_t i=0;i<include.size();i++)
                    {
                        eqtlinfo._bxz[i].reserve(slctId.size());
                        eqtlinfo._sexz[i].reserve(slctId.size());
                    }
                    for(uint32_t i=0;i<include.size();i++)
                        for(uint32_t j=0;j<slctId.size();j++)
                        {
                            eqtlinfo._bxz[i][j]=-9;
                            eqtlinfo._sexz[i][j]=-9;
                        }
                    for(uint32_t i=0;i<include.size();i++)
                    {
                        uint64_t proid=include[i];
                        uint64_t pos=eqtlinfo._cols[proid<<1];
                        uint64_t pos1=eqtlinfo._cols[(proid<<1)+1];
                        uint64_t num=pos1-pos;
                        for(int j=0;j<num;j++)
                        {
                            uint32_t rid=eqtlinfo._rowid[pos+j];
                            map<int, int>::iterator iter;
                            iter=_incld_id_map.find(rid);
                            if(iter!=_incld_id_map.end())
                            {
                                int sid=iter->second;
                                eqtlinfo._bxz[i][rk[sid]]=eqtlinfo._val[pos+j];
                                eqtlinfo._sexz[i][rk[sid]]=eqtlinfo._val[pos+j+num];
                            }
                        }
                    }
                    for (uint32_t i = 0;i <slctId.size(); i++) {
                        for(uint32_t j=0;j<include.size();j++)
                        {
                            double v_tmp=eqtlinfo._sexz[j][i];
                            if(abs(v_tmp+9)<1e-6) smr<<"NA"<<'\t'<<"NA"<<'\t';
                            else smr<<eqtlinfo._bxz[j][i]<<'\t'<<eqtlinfo._sexz[j][i]<<'\t';
                        }
                        smr<<'\n';
                    }
                }
                smr.close();
                cout << "Beta values and SE values for " << include.size() << " Probes and " << slctId.size() << " SNPs have been saved in the file [" + esdfile + "]." << endl;
            }
            
            if(make_besd_flag)
            {
                esdfile = string(outFileName)+".besd";
                FILE * smr;
                smr = fopen (esdfile.c_str(), "wb");
                if(eqtlinfo._valNum==0)
                {
                    unsigned long bsize=(include.size()*slctId.size()<<1)+1;
                    float* buffer=(float*)malloc (sizeof(float)*bsize);
                    memset(buffer,0,sizeof(float)*bsize);
                    float* ptr=buffer;
                    *ptr++=0.0; //dense flag
                    unsigned long pro_num=include.size();
                    unsigned long snp_num=slctId.size();
                    for(int i=0;i<pro_num;i++)
                    {
                        for(int j=0;j<snp_num;j++)
                            ptr[(i<<1)*snp_num+j]=eqtlinfo._bxz[include[i]][slctId[j]];
                        
                        for(int j=0;j<snp_num;j++)
                            ptr[((i<<1)+1)*snp_num+j]=eqtlinfo._sexz[include[i]][slctId[j]];
                    }
                    fwrite (buffer,sizeof(float), bsize, smr);
                    free(buffer);
                }
                else{
                    
                    vector<uint64_t> cols((include.size()<<1)+1);
                    vector<uint32_t> rowid;
                    vector<float> val;
                    vector<int> rk;
                    vector<float> tmp;
                    getRank(slctId, rk);
                    map<int, int > _incld_id_map;
                    for (int i = 0; i<slctId.size(); i++)
                    {
                        _incld_id_map.insert(pair<int, int>(slctId[i], i));
                        
                    }
                    cols[0]=0;
                    for(int i=0;i<include.size();i++)
                    {
                        tmp.clear();
                        uint64_t pos=eqtlinfo._cols[include[i]<<1];
                        uint64_t pos1=eqtlinfo._cols[(include[i]<<1)+1];
                        uint64_t num=pos1-pos;
                        uint64_t real_num=0;
                        for(uint64_t j=0;j<num;j++)
                        {
                            uint32_t rid=eqtlinfo._rowid[pos+j];
                            map<int, int>::iterator iter;
                            iter=_incld_id_map.find(rid);
                            if(iter!=_incld_id_map.end())
                            {
                                int it=iter->second;
                                val.push_back(eqtlinfo._val[pos+j]);
                                tmp.push_back(eqtlinfo._val[pos+j+num]);
                                rowid.push_back(rk[it]);
                                real_num++;
                            }
                        }
                        int ris=rowid.size()-real_num;;
                        for(uint32_t j=0;j<real_num;j++)
                        {
                            val.push_back(tmp[j]);
                            rowid.push_back(rowid[ris+j]);
                        }
                        cols[(i<<1)+1]=real_num+cols[i<<1];
                        cols[i+1<<1]=(real_num<<1)+cols[i<<1];
                    }
                    
                    uint64_t colSize=sizeof(uint64_t)*cols.size();
                    uint64_t rowSize=sizeof(uint32_t)*rowid.size();
                    uint64_t valNum=val.size();
                    uint64_t valSize=sizeof(float)*valNum;
                    uint64_t bufsize=sizeof(float)+sizeof(uint64_t)+colSize+rowSize+valSize;
                    
                    char* buffer=(char*)malloc (sizeof(char)*bufsize);
                    memset(buffer,0,sizeof(char)*bufsize);
                    float ftype=SPARSE_FILE_TYPE_3;
                    memcpy(buffer,&ftype,sizeof(float));
                    char* wptr=buffer+sizeof(float);
                    memcpy(wptr,&valNum,sizeof(uint64_t));
                    wptr+=sizeof(uint64_t);
                    memcpy(wptr,&cols[0],colSize);
                    wptr+=colSize;
                    memcpy(wptr,&rowid[0],rowSize);
                    wptr+=rowSize;
                    memcpy(wptr,&val[0],valSize);
                    fwrite (buffer,sizeof(char), bufsize, smr);
                    free(buffer);
                }
                fclose (smr);
               
                cout<<"Beta values and SE values for "<<include.size()<<" Probes and "<<slctId.size()<<" SNPs have been saved in the binary file [" + esdfile + "]." <<endl;
            }
        }
        
    }
    
    void lookup(char* outFileName,char* eqtlFileName, char* snplstName, char* problstName, char* genelistName, double plookup, bool bFlag)
    {
        eqtlInfo eqtlinfo;
        cout<<endl<<"Reading eQTL summary data..."<<endl;
        if(eqtlFileName != NULL)
        {
            read_esifile(&eqtlinfo, string(eqtlFileName)+".esi");
            if (snplstName != NULL) extract_eqtl_snp(&eqtlinfo, snplstName);
            read_epifile(&eqtlinfo, string(eqtlFileName)+".epi");
            if(problstName != NULL) extract_prob(&eqtlinfo, problstName);
            if(genelistName != NULL) extract_prob_by_gene(&eqtlinfo, genelistName);
            if(bFlag) read_besdfile(&eqtlinfo, string(eqtlFileName)+".besd");
            else      read_esdfile(&eqtlinfo, string(eqtlFileName)+".esd");
            
        }
        else throw ("Error: please input the eQTL summary information for the eQTL data files by the option --beqtl-summary.");
       
        vector<int> out_esi_id;
        vector<int> out_epi_id;
        vector<float> out_beta;
        vector<float> out_se;
        vector<double> out_pval;
        if(eqtlinfo._valNum==0)
        {
            for(uint32_t i=0;i<eqtlinfo._probNum;i++)
            {
                for(uint32_t j=0;j<eqtlinfo._snpNum;j++)
                {
                    double beta=eqtlinfo._bxz[i][j];
                    double se=eqtlinfo._sexz[i][j];
                    if(ABS(se+9)<1e-6) continue;
                    double zsxz=beta/se;
                    double pxz=pchisq(zsxz*zsxz, 1);
                    if(pxz<=plookup)
                    {
                        out_esi_id.push_back(j);
                        out_epi_id.push_back(i);
                        out_beta.push_back(beta);
                        out_se.push_back(se);
                        out_pval.push_back(pxz);
                    }
                }
            }
        }
        else
        {
            if(eqtlinfo._val.size()==0)
            {
                throw ("Error: No data extracted from the input, please check.");
            }
            
            for(uint32_t i=0;i<eqtlinfo._probNum;i++)
            {
                uint64_t proid=eqtlinfo._include[i];
                uint64_t pos=eqtlinfo._cols[proid<<1];
                uint64_t pos1=eqtlinfo._cols[(proid<<1)+1];
                uint64_t num=pos1-pos;
                for(int j=0;j<num;j++)
                {
                    double beta=eqtlinfo._val[pos+j];
                    double se=eqtlinfo._val[pos+j+num];
                    double zsxz=beta/se;
                    double pxz=pchisq(zsxz*zsxz, 1);
                    if(pxz<=plookup)
                    {
                        out_esi_id.push_back(eqtlinfo._rowid[pos+j]);
                        out_epi_id.push_back(i);
                        out_beta.push_back(beta);
                        out_se.push_back(se);
                        out_pval.push_back(pxz);
                    }
                }
            }
        }
        
        string smrfile = string(outFileName)+".lkp";
        ofstream smr(smrfile.c_str());
        if (!smr) throw ("Error: can not open the fam file " + smrfile + " to save!");
        
        smr << "SNP" <<'\t'<< "Chr" <<'\t' << "BP"  << '\t' << "A1" << '\t'<< "A2"<< '\t' << "Probe"<< '\t' << "Probe_Chr"<< '\t'<< "Probe_bp"<< '\t'<<"Gene"<<'\t'<<"b"<<'\t'<< "SE" << '\t'<<"p"<<'\n';
        
        for (int i = 0;i <out_esi_id.size(); i++) {
            smr<<eqtlinfo._esi_rs[out_esi_id[i]]<<'\t'<<eqtlinfo._esi_chr[out_esi_id[i]]<<'\t'<<eqtlinfo._esi_bp[out_esi_id[i]]<<'\t'<<eqtlinfo._esi_allele1[out_esi_id[i]]<<'\t'<<eqtlinfo._esi_allele2[out_esi_id[i]]<<'\t'<<eqtlinfo._epi_prbID[out_epi_id[i]]<<'\t'<<eqtlinfo._epi_chr[out_epi_id[i]]<<'\t'<<eqtlinfo._epi_bp[out_epi_id[i]]<<'\t'<<eqtlinfo._epi_gene[out_epi_id[i]]<<'\t'<<out_beta[i]<<'\t'<<out_se[i]<<'\t'<<out_pval[i]<< '\n';
        }
       
        smr.close();
        cout<<"Extracted results of "<<out_esi_id.size()<<" SNPs have been saved in the file [" + smrfile + "]."<<endl;
        
    }
    int get_besd_format(string besdfName)
    {
        string besdfile = besdfName + ".besd";
        char buf[8];
        ifstream besd(besdfile.c_str(), ios::in|ios::binary);
        if(!besd)
        {
            fprintf (stderr, "%s: Couldn't open file %s\n",
                     besdfile.c_str(), strerror (errno));
            exit (EXIT_FAILURE);
        }
        
        besd.read(buf, 4);
        besd.close();
        
        float* flag=(float *)buf;
        return  (int)*flag;

    }
    void combineCis(char* eqtlsmaslstName, char* outFileName,bool cisflag, int cis_itvl, int trans_itvl, float transThres, float restThres)
    {
        
        eqtlInfo eqtlinfo;
        vector<string> smasNames;
        
        read_smaslist(smasNames, string(eqtlsmaslstName));
        if(smasNames.size()==0) throw("No eqtl summary file list in [ "+ string(eqtlsmaslstName)  +" ]");
        combine_esi(&eqtlinfo, smasNames);
        vector<int> flags;
        int fileflg=get_besd_format(smasNames[0]);
        
        for(int i=1;i<smasNames.size();i++)
        {
            if(fileflg == DENSE_FILE_TYPE_1 && DENSE_FILE_TYPE_1!=get_besd_format(smasNames[i])) {
                cout<<"Erroe: You got dense file and sparse file mixed in you file list!"<<endl;
                exit(1);
            }
            if(fileflg != DENSE_FILE_TYPE_1 && DENSE_FILE_TYPE_1==get_besd_format(smasNames[i])) {
                cout<<"Erroe: You got dense file and sparse file mixed in you file list!"<<endl;
                exit(1);
            }
        }
        
        if(fileflg == DENSE_FILE_TYPE_1)
        {
            combine_epi(&eqtlinfo, smasNames);
            if(cisflag) combine_epi_esd_dense2sparse(&eqtlinfo, smasNames, string(outFileName),cis_itvl, trans_itvl, transThres, restThres);
            else combine_epi_esd_dense(&eqtlinfo, smasNames, string(outFileName));
        }
        else combine_epi_esd(&eqtlinfo, smasNames, string(outFileName));
        
    }
    
     void smr_g2g(char* gwasFileName,char* gwasFileName2,char* snplstName,char* snplst2exclde)
    {
       
       
        
    }
    
    // below for txt 2 besd
    int read_probeinfolst(probeinfolst** prbiflst,char* syllabusName)
    {
        ifstream flptr(syllabusName);
        if (!flptr) throw ("Error: can not open the file [" + string(syllabusName) + "] to read.");
        cout << "Reading eQTL probe information from [" + string(syllabusName) + "]." << endl;
        
        char buf[MAX_LINE_SIZE];
        int lineNum(0);
        while(!flptr.eof())
        {
            flptr.getline(buf,MAX_LINE_SIZE);
            lineNum++;
        }
        if(buf[0]=='\0') lineNum--;
        cout << lineNum << " Probes infomation to be included from [" + string(syllabusName) + "]." << endl;
        
        probeinfolst*  prbiflst_tmp =(probeinfolst*)malloc(lineNum*sizeof(probeinfolst));
        
        flptr.clear(ios::goodbit);
        flptr.seekg (0, ios::beg);
        for(int i=0;i<lineNum;i++)
        {
            string tmpStr;
            flptr.getline(buf,MAX_LINE_SIZE);
            
            // vector<string> vec_str;
            //  int num= split_string(buf, vec_str);
            
            istringstream iss(buf);
            iss>>tmpStr;
            prbiflst_tmp[i].probechr =atoi(tmpStr.c_str());
            iss>>tmpStr;
           // memcpy ( prbiflst_tmp[i].probeId,tmpStr.c_str(), strlen(tmpStr.c_str())+1 );
           // strcpy(prbiflst_tmp[i].probeId,tmpStr.c_str());
            strncpy(prbiflst_tmp[i].probeId,tmpStr.c_str(),MAX_LEN_PRBNAME);
            iss>>tmpStr;
            prbiflst_tmp[i].gd=atoi(tmpStr.c_str());
            iss>>tmpStr;
            prbiflst_tmp[i].bp=atoi(tmpStr.c_str());
            iss>>tmpStr;
            strncpy(prbiflst_tmp[i].genename,tmpStr.c_str(),MAX_LEN_GENENAME);
            iss>>tmpStr;
            prbiflst_tmp[i].orien=tmpStr.c_str()[0];
            iss>>tmpStr;
            prbiflst_tmp[i].snpchr=atoi(tmpStr.c_str());
            iss>>tmpStr;
            strncpy(prbiflst_tmp[i].esdpath,tmpStr.c_str(),MAX_LEN_PATH);
            tmpStr="";
            iss>>tmpStr;
            if(tmpStr.size())  strncpy(prbiflst_tmp[i].bfilepath,tmpStr.c_str(),MAX_LEN_PATH);           
        }
        *prbiflst=prbiflst_tmp;
        flptr.close();
        return lineNum;
        
    }
    void read_smr_sa(vector<string> &rs,vector<char> &a1, vector<char> &a2, vector<int> &bp, vector<float> &beta,vector<float> &se,string esdpath)
    {
        gzFile gzfile=NULL;
        ifstream flptr;
        bool gzflag=has_suffix(esdpath, "gz");
        if(gzflag)
        {
            gzfile = gzopen(esdpath.c_str(), "rb");
            if (!(gzfile)) {
                fprintf (stderr, "%s: Couldn't open file %s\n",
                         esdpath.c_str(), strerror (errno));
                exit (EXIT_FAILURE);
            }
        }
        else
        {
            flptr.open(esdpath.c_str());
            if (!flptr) throw ("Error: can not open the file [" + esdpath + "] to read.");
        }
        cout << "Reading eQTL information from [" + string(esdpath) + "]." << endl;
        
        char buf[MAX_LINE_SIZE];
        int lineNum(0);
        if(gzflag) gzgets(gzfile, buf, MAX_LINE_SIZE); //head
        else flptr.getline(buf,MAX_LINE_SIZE);// the header
        while(!flptr.eof() && !gzeof(gzfile))
        {
            string tmpStr;
            if(gzflag) gzgets(gzfile, buf, MAX_LINE_SIZE);
            else flptr.getline(buf,MAX_LINE_SIZE);
            if(buf[0]!='\0'){
                istringstream iss(buf);
                iss>>tmpStr; //SNP
                rs.push_back(tmpStr.c_str());
                iss>>tmpStr; //BP
                bp.push_back(atoi(tmpStr.c_str()));
                iss>>tmpStr; //A1
                a1.push_back(tmpStr.c_str()[0]);
                iss>>tmpStr; //A2
                a2.push_back(tmpStr.c_str()[0]);
                iss>>tmpStr; //Freq
                iss>>tmpStr; //beta
                if(!tmpStr.compare("NA")) beta.push_back(-9);
                else  beta.push_back(atof(tmpStr.c_str()));
                iss>>tmpStr; //se
                if(!tmpStr.compare("NA")) se.push_back(-9);
                else  se.push_back(atof(tmpStr.c_str()));
                iss>>tmpStr; //p
                lineNum++;
            }
        }
       
        flptr.close();
        cout << lineNum << " SNPs infomation to be included from [" + string(esdpath) + "]." << endl;
    }
    void read_plink_qassoc_gz(vector<string> &rs,vector<int> &bp, vector<float> &beta,vector<float> &se,string esdpath){
        
        cout << "Reading summary information from [" + esdpath + "]." << endl;
        char tbuf[MAX_LINE_SIZE];
        gzFile gzfile = gzopen(esdpath.c_str(), "rb");
        if (!(gzfile)) {
            fprintf (stderr, "%s: Couldn't open file %s\n",
                     esdpath.c_str(), strerror (errno));
            exit (EXIT_FAILURE);
            
        }
        
        int lineNum(0);
        gzgets(gzfile, tbuf, MAX_LINE_SIZE); //head
        while(!gzeof(gzfile))
        {
            string tmpStr;
            gzgets(gzfile, tbuf, MAX_LINE_SIZE);
            if(tbuf[0]!='\0'){
                istringstream iss(tbuf);
                iss>>tmpStr; //chr
                iss>>tmpStr; // SNP
                rs.push_back(tmpStr.c_str());
                iss>>tmpStr; //BP
                bp.push_back(atoi(tmpStr.c_str()));
                iss>>tmpStr; //MISS
                iss>>tmpStr; // beta
                beta.push_back(atof(tmpStr.c_str()));
                iss>>tmpStr; // se
                if(!tmpStr.compare("NA")) se.push_back(-9);
                else  se.push_back(atof(tmpStr.c_str()));
                lineNum++;
            }
        }
        /*
        while(!gzeof(gzfile))
        {
            gzgets(gzfile, tbuf, MAX_LINE_SIZE);
            lineNum++;
        }
        if(tbuf[0]=='\0') lineNum--;
        
        rs.resize(lineNum);
        bp.resize(lineNum);
        beta.resize(lineNum);
        se.resize(lineNum);
       
        gzrewind(gzfile);
        gzgets(gzfile, tbuf, MAX_LINE_SIZE);//head
        
        for(int i=0;i<lineNum;i++)
        {
            string tmpStr;
            gzgets(gzfile, tbuf, MAX_LINE_SIZE);
            istringstream iss(tbuf);
            iss>>tmpStr; //chr
            iss>>tmpStr; // SNP
            rs[i]=tmpStr.c_str();
            iss>>tmpStr; //BP
            bp[i]=atoi(tmpStr.c_str());
            iss>>tmpStr; //MISS
            iss>>tmpStr; // beta
            beta[i]=atof(tmpStr.c_str());
            iss>>tmpStr; // se
            if(!tmpStr.compare("NA")) se[i]=-9;
            else  se[i]=atof(tmpStr.c_str());
            iss>>tmpStr; //R2
            iss>>tmpStr; //T
            iss>>tmpStr; //P
        }
         */
        gzclose(gzfile);
        cout << lineNum << " SNPs summary info to be included from [" + esdpath + "]." << endl;
    }
    
    void read_plink_qassoc(vector<string> &rs,vector<int> &bp, vector<float> &beta,vector<float> &se,string esdpath){
        
        ifstream flptr(esdpath.c_str());
        if (!flptr) throw ("Error: can not open the file [" + string(esdpath) + "] to read.");
        cout << "Reading eQTL information from [" + string(esdpath) + "]." << endl;
        
        char buf[MAX_LINE_SIZE];
        int lineNum(0);
        flptr.getline(buf,MAX_LINE_SIZE);
        while(!flptr.eof())
        {
            flptr.getline(buf,MAX_LINE_SIZE);
            lineNum++;
        }
        if(buf[0]=='\0') lineNum--;
        cout << lineNum << " SNPs infomation to be included from [" + string(esdpath) + "]." << endl;
        rs.resize(lineNum);
        bp.resize(lineNum);
        beta.resize(lineNum);
        se.resize(lineNum);
        
        flptr.clear(ios::goodbit);
        flptr.seekg (0, ios::beg);
        flptr.getline(buf,MAX_LINE_SIZE);
        for(int i=0;i<lineNum;i++)
        {
            string tmpStr;
            flptr.getline(buf,MAX_LINE_SIZE);
            istringstream iss(buf);
            iss>>tmpStr; //chr
            iss>>tmpStr; // SNP
            rs[i]=tmpStr.c_str();
            iss>>tmpStr; //BP
            bp[i]=atoi(tmpStr.c_str());
            iss>>tmpStr; //MISS
            iss>>tmpStr; // beta
            beta[i]=atof(tmpStr.c_str());
            iss>>tmpStr; // se
            if(!tmpStr.compare("NA")) se[i]=-9;
            else  se[i]=atof(tmpStr.c_str());
            iss>>tmpStr; //R2
            iss>>tmpStr; //T
            iss>>tmpStr; //P
        }
        flptr.close();
    }
    void read_gemma_qassoc(vector<string> &rs,vector<int> &bp, vector<char> &a1, vector<char> &a2, vector<float> &beta,vector<float> &se,string esdpath)
    {
        gzFile gzfile=NULL;
        ifstream flptr;
        bool gzflag=has_suffix(esdpath, "gz");
        if(gzflag)
        {
            gzfile = gzopen(esdpath.c_str(), "rb");
            if (!(gzfile)) {
                fprintf (stderr, "%s: Couldn't open file %s\n",
                         esdpath.c_str(), strerror (errno));
                exit (EXIT_FAILURE);
            }
        }
        else
        {
            flptr.open(esdpath.c_str());
            if (!flptr) throw ("Error: can not open the file [" + esdpath + "] to read.");
        }
        cout << "Reading eQTL information from [" + string(esdpath) + "]." << endl;
        
        char buf[MAX_LINE_SIZE];
        int lineNum(0);
        if(gzflag) gzgets(gzfile, buf, MAX_LINE_SIZE); //head
        else flptr.getline(buf,MAX_LINE_SIZE);// the header
        
        while(!flptr.eof() && !gzeof(gzfile))
        {
            string tmpStr;
            if(gzflag) gzgets(gzfile, buf, MAX_LINE_SIZE);
            else flptr.getline(buf,MAX_LINE_SIZE);
            if(buf[0]!='\0'){
                istringstream iss(buf);
                iss>>tmpStr; //chr
                iss>>tmpStr; // rs
                rs.push_back(tmpStr.c_str());
                iss>>tmpStr; // ps (np)
                bp.push_back(atoi(tmpStr.c_str()));
                iss>>tmpStr; // n_miss
                iss>>tmpStr; // allel1: minor allele
                a2.push_back(tmpStr.c_str()[0]);
                iss>>tmpStr; // allele0: major allele
                a1.push_back(tmpStr.c_str()[0]);
                iss>>tmpStr; // af
                iss>>tmpStr; //beta
                beta.push_back(atof(tmpStr.c_str()));
                iss>>tmpStr; //se
                if(!tmpStr.compare("NA")) se.push_back(-9);
                else  se.push_back(atof(tmpStr.c_str()));
                iss>>tmpStr; //l_remle
                iss>>tmpStr; //p_wald
                lineNum++;

            }
        }
       
        cout << lineNum << " SNPs infomation to be included from [" + string(esdpath) + "]." << endl;
        if(gzflag) gzclose(gzfile);
        else flptr.close();
    }
    void read_merlin_fastassoc(vector<string> &rs,vector<int> &bp, vector<char> &a1, vector<char> &a2, vector<float> &beta,vector<float> &se,string esdpath)
    {
        cout << "Reading summary information from [" + esdpath + "]." << endl;
      
        gzFile gzfile=NULL;
        ifstream flptr;
        bool gzflag=has_suffix(esdpath, "gz");
        if(gzflag)
        {
            gzfile = gzopen(esdpath.c_str(), "rb");
            if (!(gzfile)) {
                fprintf (stderr, "%s: Couldn't open file %s\n",
                         esdpath.c_str(), strerror (errno));
                exit (EXIT_FAILURE);
            }
        }
        else
        {
            flptr.open(esdpath.c_str());
            if (!flptr) throw ("Error: can not open the file [" + esdpath + "] to read.");
        }
        cout << "Reading eQTL information from [" + string(esdpath) + "]." << endl;
        char tbuf[MAX_LINE_SIZE];
        
        int lineNum(0);
       
        if(gzflag) gzgets(gzfile, tbuf, MAX_LINE_SIZE); //head
        else flptr.getline(tbuf,MAX_LINE_SIZE);// the header
        while(!flptr.eof() && !gzeof(gzfile))
        {
            string tmpStr;
            if(gzflag) gzgets(gzfile, tbuf, MAX_LINE_SIZE);
            else flptr.getline(tbuf,MAX_LINE_SIZE);
            if(tbuf[0]!='\0'){
                istringstream iss(tbuf);
                iss>>tmpStr; //chr
                iss>>tmpStr; // SNP
                rs.push_back(tmpStr.c_str());
                iss>>tmpStr; //A1
                iss>>tmpStr; //A2
                iss>>tmpStr; //Freq
                iss>>tmpStr; //Trait
                iss>>tmpStr; // beta
                beta.push_back(atof(tmpStr.c_str()));
                iss>>tmpStr; // se
                if(!tmpStr.compare("NA")) se.push_back(-9);
                else  se.push_back(atof(tmpStr.c_str()));
                lineNum++;
            }
        }
        if(gzflag) gzclose(gzfile);
        else flptr.close();
        cout << lineNum << " SNPs summary info to be included from [" + esdpath + "]." << endl;

     }
    //sort in ascend order
    int comp(const void *a,const void *b){ return (*(probeinfolst *)a).snpchr>(*(probeinfolst *)b).snpchr?1:-1; }
    
    void get_curCHR_esi_info(vector<string> &_esi_rs, vector<int> &_esi_bp, vector<int> &_esi_gd,vector<char> &_esi_a1, vector<char> &_esi_a2,probeinfolst** prbiflst,int cur_chr, int rss,int prb_num_cur_chr,int fformat)
    {
        probeinfolst* locinfolst=*prbiflst;
        map<string, int> rs_map;
        long size = 0;
        _esi_rs.clear();
        _esi_bp.clear();
        _esi_gd.clear();
        _esi_a2.clear();
        _esi_a1.clear();
        char buf[MAX_LINE_SIZE];
        for(int j=0;j<prb_num_cur_chr;j++)
        {
            string esdfilename=(locinfolst+rss+j)->esdpath;
            gzFile gzfile ;
            ifstream flptr;
            bool gzflag=has_suffix(esdfilename, "gz");
            if(gzflag)
            {
                gzfile = gzopen(esdfilename.c_str(), "rb");
                if (!(gzfile)) {
                    fprintf (stderr, "%s: Couldn't open file %s\n",
                             esdfilename.c_str(), strerror (errno));
                    exit (EXIT_FAILURE);
                }
            }
            else
            {
                flptr.open((locinfolst+rss+j)->esdpath);
                if (!flptr) throw ("Error: can not open the file [" + string((locinfolst+rss+j)->esdpath) + "] to read.");
            }
           
            if(gzflag) gzgets(gzfile, buf, MAX_LINE_SIZE); //head
            else flptr.getline(buf,MAX_LINE_SIZE);// the header
            if(fformat==0)
            {
                while(!flptr.eof() && !gzeof(gzfile))
                {
                    if(buf[0]!='\0')
                    {
                        string tmpStr;
                        if(gzflag) gzgets(gzfile, buf, MAX_LINE_SIZE);
                        else flptr.getline(buf,MAX_LINE_SIZE);
                        istringstream iss(buf);
                        iss>>tmpStr; //SNP
                        
                        if(tmpStr[0]!='\0') rs_map.insert(pair<string, int>(tmpStr.c_str(), cur_chr));
                        if (size < rs_map.size())
                        {
                            _esi_rs.push_back(tmpStr.c_str());
                            iss>>tmpStr; //BP
                            _esi_bp.push_back(atoi(tmpStr.c_str()));
                            iss>>tmpStr; //A1
                            _esi_a1.push_back(tmpStr.c_str()[0]);
                            iss>>tmpStr; //A2
                            _esi_a2.push_back(tmpStr.c_str()[0]);
                            _esi_gd.push_back(0);
                            size = rs_map.size();
                        }
                    }
                }
            }
            else if(fformat==1)
            {
                while(!flptr.eof() && !gzeof(gzfile))
                {
                    if(buf[0]!='\0')
                    {
                        string tmpStr;
                        if(gzflag) gzgets(gzfile, buf, MAX_LINE_SIZE);
                        else flptr.getline(buf,MAX_LINE_SIZE);
                        istringstream iss(buf);
                        iss>>tmpStr; //chr
                        iss>>tmpStr; //rs
                        if(tmpStr[0]!='\0') rs_map.insert(pair<string, int>(tmpStr.c_str(), cur_chr));
                        if (size < rs_map.size())
                        {
                            _esi_rs.push_back(tmpStr.c_str());
                            iss>>tmpStr; //BP
                            _esi_bp.push_back(atoi(tmpStr.c_str()));
                            _esi_gd.push_back(0);
                            size = rs_map.size();
                        }
                    }
                }
            }
            else if(fformat==2)
            {
                while(!flptr.eof() && !gzeof(gzfile) )
                {
                    if(buf[0]!='\0')
                    {
                        string tmpStr;
                        if(gzflag) gzgets(gzfile, buf, MAX_LINE_SIZE);
                        else flptr.getline(buf,MAX_LINE_SIZE);
                        istringstream iss(buf);
                        iss>>tmpStr; //chr
                        iss>>tmpStr; //rs
                        if(tmpStr[0]!='\0') rs_map.insert(pair<string, int>(tmpStr.c_str(), cur_chr));
                        if (size < rs_map.size())
                        {
                            _esi_rs.push_back(tmpStr.c_str());
                            iss>>tmpStr; //BP
                            _esi_bp.push_back(atoi(tmpStr.c_str()));
                            iss>>tmpStr;//n_miss
                            iss>>tmpStr; //A1 minor allele
                            _esi_a1.push_back(tmpStr.c_str()[0]);
                            iss>>tmpStr; //A0 major allele
                            _esi_a2.push_back(tmpStr.c_str()[0]);
                            
                            _esi_gd.push_back(0);
                            size = rs_map.size();
                        }
                    }
                }
            }
            else if(fformat==3)
            {
                while(!flptr.eof() && !gzeof(gzfile) )
                {
                    if(buf[0]!='\0')
                    {
                        string tmpStr;
                        if(gzflag) gzgets(gzfile, buf, MAX_LINE_SIZE);
                        else flptr.getline(buf,MAX_LINE_SIZE);
                        istringstream iss(buf);
                        iss>>tmpStr; //chr
                        iss>>tmpStr; //rs
                        if(tmpStr[0]!='\0') rs_map.insert(pair<string, int>(tmpStr.c_str(), cur_chr));
                        if (size < rs_map.size())
                        {
                            _esi_rs.push_back(tmpStr.c_str());
                            iss>>tmpStr; //A1 minor allele
                            _esi_a1.push_back(tmpStr.c_str()[0]);
                            iss>>tmpStr; //A0 major allele
                            _esi_a2.push_back(tmpStr.c_str()[0]);
                            _esi_gd.push_back(0);
                            size = rs_map.size();
                        }
                    }
                }
            }

            if(gzflag) gzclose(gzfile);
            else flptr.close();
        }
        if(fformat==1 )
        {
            vector<int> rsid;
            bInfo binfo;
            string bimfname=string((locinfolst+rss)->bfilepath)+".bim";
            read_bimfile(&binfo, bimfname);
            match_only(_esi_rs,binfo._snp_name,rsid); //general speaking, _esi_rs is the subset of bimrs.
            long rsnum=rsid.size();
            long _rsnum=_esi_rs.size();
            if(rsnum==_rsnum)
            {
                for(int k=0;k<rsid.size();k++) _esi_a1.push_back(binfo._allele1[rsid[k]]);
                for(int k=0;k<rsid.size();k++) _esi_a2.push_back(binfo._allele2[rsid[k]]);
            }
            else
            {
                printf("Not all SNPs in eQTL summary data files are in genotype Bim file. Please check!");
                exit(1);
            }
        }
        
        if(fformat==3 ) //only deal with BSGS methylation summary data
        {
            vector<int> rsid;
            vector<string> snp_name;
            vector<int> snp_bp;
            string mapfname=string((locinfolst+rss)->bfilepath)+".map";
            ifstream mapptr(mapfname.c_str());
            if (!mapptr) throw ("Error: can not open the file [" + mapfname + "] to read.");
            while(!mapptr.eof())
            {
                
                string tmpStr;
                mapptr.getline(buf,MAX_LINE_SIZE);
                if(buf[0]!='\0')
                {
                    istringstream iss(buf);
                    iss>>tmpStr; //chr
                    iss>>tmpStr; //rs
                    snp_name.push_back(tmpStr.c_str());
                    iss>>tmpStr; //bp
                    snp_bp.push_back(atoi(tmpStr.c_str()));
                }
            }
            mapptr.close();
            match_only(_esi_rs,snp_name,rsid);
            
            long rsnum=rsid.size();
            long _rsnum=_esi_rs.size();
            if(rsnum==_rsnum)
            {
             
                for(int k=0;k<rsid.size();k++) _esi_bp.push_back(snp_bp[rsid[k]]);
            }
            else
            {
                printf("Not all SNPs in eQTL summary data files are in genotype Bim file. Please check!");
                exit(1);
            }
        }
    }

   void make_besd(char*outFileName, char* syllabusName, bool gctaflag,bool plinkflag,bool gemmaflag,bool merlinflag)
    {
        if(syllabusName==NULL) throw("Error: please input eQTL syllabus file by the flag --eqtl-flist.");
        int flagcount=0, fformat=0;
        if(gctaflag){ flagcount++; fformat=0;}
        if(plinkflag){ flagcount++;fformat=1;}
        if(gemmaflag){ flagcount++; fformat=2;}
        if(merlinflag){ flagcount++; fformat=3;}
        if(flagcount==0 || flagcount>1) throw("Error: please verify the file format flags. Only one flag can be specified.");
        probeinfolst* prbiflst;
        int probeNum=read_probeinfolst(&prbiflst,syllabusName);
        qsort(prbiflst,probeNum,sizeof(probeinfolst),comp);        
        //get unique chr
        map<int, int> snp_chr_map;
        for(int i=0;i<probeNum;i++)
        {
            snp_chr_map.insert(pair<int, int>(prbiflst[i].snpchr, i));
           
        }
        long ttl_chr_num=snp_chr_map.size();
        map<int, int>::iterator iter=snp_chr_map.begin();
        vector<int> unique_chr;
        vector<int> beginRid;
        for (iter = snp_chr_map.begin(); iter != snp_chr_map.end(); iter++)
        {
            unique_chr.push_back(iter->first);
            beginRid.push_back(iter->second);
        }
        beginRid.push_back(probeNum);
        for(int i=0;i<ttl_chr_num;i++)
        {
            int cur_chr=unique_chr[i];
            int rss=beginRid[i];
            int rts=beginRid[i+1];
            int prb_num_cur_chr=rts-rss;
            
            //get epi
            string epifile = string(outFileName)+".chr"+atos(cur_chr)+string(".epi");
            ofstream epi(epifile.c_str());
            if (!epi) throw ("Error: can not open the epi file " + epifile + " to save!");
            for (int j = 0;j <prb_num_cur_chr; j++) {
                epi<<prbiflst[rss+j].probechr<<'\t'<<prbiflst[rss+j].probeId<<'\t'<<0<<'\t'<<prbiflst[rss+j].bp<<'\t'<<prbiflst[rss+j].genename<<'\t'<<prbiflst[rss+j].orien<<'\n';
            }
            epi.close();
            cout<<prb_num_cur_chr<<" probes have been saved in the file [" + epifile + "]."<<endl;
            
            //get esi: from bim file
            vector<string> _esi_rs;
            vector<int> _esi_bp;
             vector<int> _esi_gd;
            vector<char> _esi_a1;
            vector<char> _esi_a2;
            get_curCHR_esi_info(_esi_rs,_esi_bp,_esi_gd,_esi_a1,_esi_a2,&prbiflst,cur_chr,rss,prb_num_cur_chr,fformat);
            
            string esifile =  string(outFileName)+".chr"+atos(cur_chr)+string(".esi");
            ofstream esi(esifile.c_str());
            if (!esi) throw ("Error: can not open the esi file to save!");
            for (int j = 0;j <_esi_rs.size(); j++) {
                esi<<cur_chr<<'\t'<<_esi_rs[j]<<'\t'<<_esi_gd[j]<<'\t'<<_esi_bp[j]<<'\t'<<_esi_a1[j]<<'\t'<<_esi_a2[j]<<'\n';
            }
            esi.close();
            cout<<_esi_rs.size()<<" SNPs have been saved in the file [" + esifile + "]."<<endl;
            long snp_num_cur_chr=_esi_rs.size();
            long snp_num_saved = snp_num_cur_chr;
            
            
            // get esd info
            uint64_t bsize=((uint64_t)snp_num_cur_chr*prb_num_cur_chr<<1)+1;
            float* buffer=(float*)malloc (sizeof(float)*bsize);
            if (NULL == buffer) {
                fprintf(stderr, "malloc failed\n");
                exit(-1);
            }
            for(int j=0;j<bsize;j++) buffer[j]=-9; //init
           
            float* ptr=buffer;
            *ptr++=0.0; //dense
            for(int j=0;j<prb_num_cur_chr;j++)
            {
                vector<string> _rs;
                vector<float> _beta;
                vector<float> _se;
                vector<int> _bp;
                vector<char> _a1;
                vector<char> _a2;
                string zname=prbiflst[rss+j].esdpath;
                bool gzflag=has_suffix(zname, "gz");
                switch (fformat)
                {
                    case 0:
                        read_smr_sa(_rs, _a1, _a2,_bp, _beta, _se, prbiflst[rss+j].esdpath);
                        break;
                    case 1:
                        if(gzflag) read_plink_qassoc_gz(_rs, _bp, _beta, _se, prbiflst[rss+j].esdpath);
                        else read_plink_qassoc(_rs, _bp, _beta, _se, prbiflst[rss+j].esdpath);
                        break;
                    case 2:
                        read_gemma_qassoc(_rs,_bp, _a1, _a2, _beta, _se, prbiflst[rss+j].esdpath);
                        break;
                    case 3:
                        read_merlin_fastassoc(_rs,_bp, _a1, _a2, _beta, _se, prbiflst[rss+j].esdpath);
                        break;
                }
                
                vector<int> rsid;
                match_only(_rs,_esi_rs,rsid); //general speaking, _rs is the subset of binfo._snp_name. but in case that some users mismatched dataset, common SNPs should be extracted.
                if(rsid.size()==_rs.size())
                {
                    for(int k=0;k<rsid.size();k++) ptr[(j<<1)*snp_num_cur_chr+rsid[k]]=_beta[k];
                    for(int k=0;k<rsid.size();k++) ptr[((j<<1)+1)*snp_num_cur_chr+rsid[k]]=_se[k];
                }
                else
                {
                    vector<string> commsnp;
                    for(int k=0;k<rsid.size();k++) commsnp.push_back(_esi_rs[rsid[k]]);
                    vector<int> _rsid;
                    match_only(commsnp,_rs,_rsid);
                    for(int k=0;k<rsid.size();k++) ptr[(j<<1)*snp_num_cur_chr+rsid[k]]=_beta[_rsid[k]];
                    for(int k=0;k<rsid.size();k++) ptr[((j<<1)+1)*snp_num_cur_chr+rsid[k]]=_se[_rsid[k]];
                }
                snp_num_saved=rsid.size();
            }
            
            string esdfile=string(outFileName)+".chr"+atos(cur_chr)+string(".besd");
            FILE * smr1;
            smr1 = fopen (esdfile.c_str(), "wb");
            fwrite (buffer,sizeof(float), bsize, smr1);
            fclose (smr1);
            
            free(buffer);
             cout<<"Beta values and SE values for "<<prb_num_cur_chr<<" Probes and "<<snp_num_saved<<" SNPs have been saved in the binary file [" + esdfile + "]." <<endl;
        }
       
       
       if(prbiflst!=NULL) free(prbiflst);
        
    }
    
     void smr_e2e(char* outFileName, char* bFileName,char* eqtlFileName, char* eqtlFileName2, double maf,char* indilstName, char* snplstName,char* problstName, char* oproblstName,char* eproblstName,bool bFlag,double p_hetero,double ld_top,int m_hetero, char* indilst2remove, char* snplst2exclde, char* problst2exclde, char* oproblst2exclde,char* eproblst2exclde,double p_smr,char* refSNP, bool heidioffFlag,int cis_itvl,char* traitlstName,bool plotflg,int op_wind)
    {
        setNbThreads(thread_num);
        
        eqtlInfo etrait;
        eqtlInfo esdata;
        bInfo bdata;
        double threshold= chi_val(1,p_hetero);
        bool heidiFlag=false;
        cis_itvl=cis_itvl*1000;
        
        if(!heidioffFlag && bFileName == NULL ) throw("Error: please input Plink file for SMR analysis by the flag --bfile.");
        if(eqtlFileName==NULL) throw("Error: please input eQTL summary data for SMR analysis by the flag --eqtl-summary.");
        if(refSNP!=NULL) heidiFlag=true;
        if(problstName != NULL) cout<<"WARNING: --extract-probe here presumes the probe list should contain both probes of exposure dataset and probes of outcome dataset.\n If you want to only extract probes from one dataset please include these probles in the file and all the probes of the other dataset as well.\n"<<endl;
        read_esifile(&etrait, string(eqtlFileName)+".esi");
        if (snplstName != NULL) extract_eqtl_snp(&etrait, snplstName);
        if(snplst2exclde != NULL) exclude_eqtl_snp(&etrait, snplst2exclde);
        read_epifile(&etrait, string(eqtlFileName)+".epi");
        if(problstName != NULL) extract_prob(&etrait, problstName);
        if(problst2exclde != NULL) exclude_prob(&etrait, problst2exclde);
        if(oproblstName != NULL ) extract_prob(&etrait, oproblstName);
        if(oproblst2exclde != NULL) exclude_prob(&etrait, oproblst2exclde);
        
        
        if(bFlag) read_besdfile(&etrait, string(eqtlFileName)+".besd");
        else      read_esdfile(&etrait, string(eqtlFileName)+".esd");
        
        read_esifile(&esdata, string(eqtlFileName2)+".esi");
        if (snplstName != NULL) extract_eqtl_snp(&esdata, snplstName);
        if(snplst2exclde != NULL) exclude_eqtl_snp(&esdata, snplst2exclde);
        if(!heidioffFlag)
        {
            read_famfile(&bdata, string(bFileName)+".fam");
            if(indilstName != NULL) keep_indi(&bdata,indilstName);
            if(indilst2remove != NULL) remove_indi(&bdata, indilst2remove);
            read_bimfile(&bdata, string(bFileName)+".bim");
            if(snplstName != NULL) extract_snp(&bdata, snplstName);
            if(snplst2exclde != NULL) exclude_snp(&bdata, snplst2exclde);
            allele_check(&bdata, &etrait, &esdata);
            // if no snp left after check
            read_bedfile(&bdata, string(bFileName)+".bed");
            if (bdata._mu.empty()) calcu_mu(&bdata);
            if (maf > 0)
            {
                filter_snp_maf(&bdata, maf);
                update_geIndx(&bdata, &etrait, &esdata);
            }
            
        }else
        {
            allele_check(&etrait, &esdata);
        }
       
        //the etrait is not updated, so from now on _esi_include should be used always.
        cout<<"Reading eQTL summary data..."<<endl;
        read_epifile(&esdata, string(eqtlFileName2)+".epi");
        if(problstName != NULL) extract_prob(&esdata, problstName);
        if(problst2exclde != NULL) exclude_prob(&esdata, problst2exclde);
        if(eproblstName != NULL ) extract_prob(&esdata, eproblstName);
        if(eproblst2exclde != NULL) exclude_prob(&esdata, eproblst2exclde);       
        
        if(bFlag) read_besdfile(&esdata, string(eqtlFileName2)+".besd");
        else      read_esdfile(&esdata, string(eqtlFileName2)+".esd");
        /********************/
        /*
        for(int z=0;z<esdata._snpNum ;z++)
        {
            string esrs=esdata._esi_rs[z];
            string etrs=etrait._esi_rs[etrait._esi_include[z]];
            string brs=bdata._snp_name[bdata._include[z]];
            bool flg=(esrs==etrs) && (esrs==brs) && (etrs==brs);
            if(!flg) cout<<"err"<<endl;
        }
         */
        /********************/
         /********************/
        /*
        string trs="rs393270"; //"rs413592";
        string prnm="cg05202042";
        long pos=find(esdata._esi_rs.begin(), esdata._esi_rs.end(), trs)-esdata._esi_rs.begin();
        long pos2=find(esdata._epi_prbID.begin(), esdata._epi_prbID.end(), prnm)-esdata._epi_prbID.begin();
        uint64_t beta_start=esdata._cols[pos2<<1];
        uint64_t se_start=esdata._cols[1+(pos2<<1)];
        uint64_t numsnps=se_start-beta_start;
        for(int j=0;j<numsnps;j++)
        {
            int ge_rowid=esdata._rowid[beta_start+j];
            if(ge_rowid==pos) cout<<esdata._val[beta_start+j]<<":"<<esdata._val[se_start+j]<<endl;
        }
         */
         /********************/
        
        for( int ii=0;ii<etrait._probNum;ii++)
        {
            
            gwasData gdata;
            gdata.allele_1=(char*)malloc(etrait._esi_include.size()*sizeof(char));
            gdata.allele_2=(char*)malloc(etrait._esi_include.size()*sizeof(char));
            gdata.byz=(double*)malloc(etrait._esi_include.size()*sizeof(double));
            gdata.seyz=(double*)malloc(etrait._esi_include.size()*sizeof(double));
            gdata.freq=(double*)malloc(etrait._esi_include.size()*sizeof(double));
            gdata.pvalue=(double*)malloc(etrait._esi_include.size()*sizeof(double));
            gdata.splSize=(uint32_t*)malloc(etrait._esi_include.size()*sizeof(uint32_t));
            
            
            string traitname=etrait._epi_prbID[ii];
            cout<<"\nPerforming analysis of eTrait [ "+traitname+" ]..."<<endl;
            memset(gdata.byz,0,etrait._esi_include.size()*sizeof(double));
            memset(gdata.seyz,0,etrait._esi_include.size()*sizeof(double));
            memset(gdata.allele_1,0,etrait._esi_include.size()*sizeof(char));
            memset(gdata.allele_2,0,etrait._esi_include.size()*sizeof(char));
            gdata._include.clear();
            gdata.snpName.clear();
            int count=0;
            if(etrait._rowid.empty())
            {
                for (int j = 0; j<etrait._esi_include.size(); j++)
                {
                    if (abs(etrait._bxz[ii][etrait._esi_include[j]] + 9) > 1e-6)
                    {
                        gdata.byz[count]=etrait._bxz[ii][etrait._esi_include[j]];
                        gdata.seyz[count]=etrait._sexz[ii][etrait._esi_include[j]];
                        gdata.snpName.push_back(etrait._esi_rs[etrait._esi_include[j]]);
                        gdata.allele_1[count]=etrait._esi_allele1[etrait._esi_include[j]];
                        gdata.allele_2[count]=etrait._esi_allele2[etrait._esi_include[j]];
                        gdata._include.push_back(etrait._esi_include[j]); // row id selected
                        count++;
                    }
                }
            }
            else
            {
                uint64_t beta_start=etrait._cols[ii<<1];
                uint64_t se_start=etrait._cols[1+(ii<<1)];
                uint64_t numsnps=se_start-beta_start;
                for(int j=0;j<numsnps;j++)
                {
                    int ge_rowid=etrait._rowid[beta_start+j];
                    if(binary_search(etrait._esi_include.begin(), etrait._esi_include.end(), ge_rowid))
                    {
                        gdata.byz[count]=etrait._val[beta_start+j];
                        gdata.seyz[count]=etrait._val[se_start+j];
                        gdata.snpName.push_back(etrait._esi_rs[ge_rowid]);
                        gdata.allele_1[count]=etrait._esi_allele1[ge_rowid];
                        gdata.allele_2[count]=etrait._esi_allele2[ge_rowid];
                        gdata._include.push_back(ge_rowid);
                        count++;
                    }
                    
                }
            }
            if(gdata.snpName.size()< m_hetero)
            {
                cout<<gdata.snpNum<<" common SNPs (less than parameter m_hetero: "+atos(m_hetero)+" ) are included from eTrait [ "+traitname+" ] summary."<<endl;
                continue;
            }
            gdata.snpNum=gdata.snpName.size();
            cout<<gdata.snpNum<<" common SNPs are included from eTrait [ "+traitname+" ] summary."<<endl;
            
            int outcome_probe_wind=op_wind*1000;
            int traitbp=etrait._epi_bp[ii];
            int lowerbounder=(traitbp-outcome_probe_wind)>0?(traitbp-outcome_probe_wind):0;
            int upperbounder=traitbp+outcome_probe_wind;
            int traitchr=etrait._epi_chr[ii];
            vector<int> icld_tmp;
            for(int j=0;j<esdata._include.size();j++)
            {
                int bptmp=esdata._epi_bp[esdata._include[j]];
                if(esdata._epi_chr[esdata._include[j]]==traitchr && bptmp>=lowerbounder && bptmp<=upperbounder) icld_tmp.push_back(esdata._include[j]);
            }

            int outCount = -1;
            unsigned int probNum = icld_tmp.size();
           // unsigned int probNum = esdata._probNum;
            // vectors for output
            vector<long> out_probid(probNum);
            vector<string> bxy(probNum); // origin is double
            vector<string> sexy(probNum);// origin is double
            vector<string> pxy(probNum); // origin is double
            vector<string> bgwas(probNum); // origin is double
            vector<string> beqtl(probNum); // origin is double
            vector<string> segwas(probNum); // origin is double
            vector<string> seeqtl(probNum); // origin is double
            vector<string> pgwas(probNum); // origin is double
            vector<string> peqtl(probNum); // origin is double
            vector<string> rsid(probNum);
            vector<string> rschr(probNum);
            vector<char> rsa1(probNum);
            vector<char> rsa2(probNum);
            vector<string> rsbp(probNum); //origin is unsigned int
            vector<string> rsfreq(probNum); //origin is unsigned double
            vector<string> prb1(probNum); // origin is double
            vector<string> nsnp_test1(probNum);
            vector<string> top_match1(probNum);  // origin is int
            vector<string> ldrsq(probNum); // origin is double
          
            
            cout<<endl<<"Performing SMR and heterogeneity analysis..... "<<endl;
            float progr0=0.0 , progr1;
            progress_print(progr0);
            
            
            vector<double> bxz;
            vector<double> sexz;
            vector<uint32_t> curId;
            vector<string> eName;
            vector<int> snpchrom;
            
            vector<char> allele1;
            vector<char> allele2;
            vector<uint32_t> bpsnp;
            vector<double> freq;
            
            vector<double> byz;
            vector<double> seyz;
            VectorXd zsxz;
            
            vector<int> sn_ids;
            
            VectorXd _byz;
            VectorXd _seyz;
            VectorXd _bxz;
            VectorXd _sexz;
            VectorXd _zsxz;
            MatrixXd _X;
            MatrixXd _LD;
            VectorXd ld_v;
            MatrixXd _LD_heidi;
            MatrixXd _X_heidi;
            
            //for plot
            vector<string> plot_paths;
            for(int ti=0;ti<icld_tmp.size();ti++)
            {
                int i=icld_tmp[ti];
                progr1=1.0*ti/probNum;
                if(progr1-progr0-0.05>1e-6 || ti+1==probNum)
                {
                    if(ti+1==probNum) progr1=1.0;
                    progress_print(progr1);
                    progr0=progr1;
                }
                
                // for plot
                vector<uint32_t> plot_snpidx;
                vector<uint32_t> plot_probeidx;
                vector<double> plot_bxz;
                vector<double> plot_sexz;
                int plotdir_id=ti>>10;
                string plotdir="";
                
                //extract info from eqtl summary and gwas summary
                bxz.clear();
                sexz.clear();
                curId.clear(); // is the idxes of bfile._include not the values of
                eName.clear();
                snpchrom.clear();
                byz.clear();
                seyz.clear();
                allele1.clear();
                allele2.clear();
                bpsnp.clear();
                freq.clear();
                long maxid =-9;
                int probebp=esdata._epi_bp[i];
                int probechr=esdata._epi_chr[i];
                if(esdata._rowid.empty())
                {
                    for (int j = 0; j<esdata._esi_include.size(); j++)// bdata._include.size() == esdata._esi_include.size() == etrait._esi_include.size()
                    {
                        if (abs(esdata._bxz[i][j] + 9) > 1e-6)
                        {
                            int snpbp=esdata._esi_bp[j];
                            int snpchr=esdata._esi_chr[j];
                            
                            int etrait_rid=etrait._esi_include[j];
                            long pos=find(gdata._include.begin(), gdata._include.end(), etrait_rid)-gdata._include.begin();
                            
                            if(snpchr==probechr && ABS(probebp-snpbp)<=cis_itvl && pos!=gdata._include.size())
                            {
                                bxz.push_back(esdata._bxz[i][j]);
                                sexz.push_back(esdata._sexz[i][j]);
                                byz.push_back(gdata.byz[pos]);
                                seyz.push_back(gdata.seyz[pos]);
                                curId.push_back(j);
                                eName.push_back(esdata._esi_rs[j]);
                                snpchrom.push_back(esdata._esi_chr[j]);
                                if(heidiFlag && esdata._esi_rs[j]==string(refSNP)) maxid=(eName.size()-1);
                                if(!heidioffFlag) //if heidi off , bfile is not necessary to read.
                                {
                                    freq.push_back(bdata._mu[bdata._include[j]]/2);
                                }
                                allele1.push_back(esdata._esi_allele1[j]);
                                allele2.push_back(esdata._esi_allele2[j]);
                                bpsnp.push_back(esdata._esi_bp[j]);
                            }
                        }
                    }
                    
                }
                else{
                    uint64_t beta_start=esdata._cols[i<<1];
                    uint64_t se_start=esdata._cols[1+(i<<1)];
                    uint64_t numsnps=se_start-beta_start;
                    for(int j=0;j<numsnps;j++)
                    {
                        int ge_rowid=esdata._rowid[beta_start+j];
                        int snpbp=esdata._esi_bp[ge_rowid];
                        int snpchr=esdata._esi_chr[ge_rowid];
                        
                        int etrait_rid=etrait._esi_include[ge_rowid];
                        long pos=find(gdata._include.begin(), gdata._include.end(), etrait_rid)-gdata._include.begin();
                        /****the same **/
                        //string ers=esdata._esi_rs[ge_rowid];
                        //string trs=etrait._esi_rs[etrait._esi_include[ge_rowid]];
                        //long pos=find(gdata.snpName.begin(), gdata.snpName.end(), trs)-gdata.snpName.begin();
                        /******/
                        if(snpchr==probechr && ABS(probebp-snpbp)<=cis_itvl && pos!=gdata._include.size())
                        {
                            bxz.push_back(esdata._val[beta_start+j]);
                            sexz.push_back(esdata._val[se_start+j]);
                            byz.push_back(gdata.byz[pos]);
                            seyz.push_back(gdata.seyz[pos]);
                            curId.push_back(ge_rowid);
                            eName.push_back(esdata._esi_rs[ge_rowid]);
                            snpchrom.push_back(esdata._esi_chr[ge_rowid]);
                            if(heidiFlag && esdata._esi_rs[ge_rowid]==string(refSNP)) maxid=(eName.size()-1);
                            allele1.push_back(esdata._esi_allele1[ge_rowid]);
                            allele2.push_back(esdata._esi_allele2[ge_rowid]);
                            bpsnp.push_back(esdata._esi_bp[ge_rowid]);
                            if(!heidioffFlag){
                                freq.push_back(bdata._mu[bdata._include[ge_rowid]]/2);
                            }
                        }
                    }
                }
                if(heidiFlag && maxid==-9) continue; //heidi SNP is not in selected SNPs
                if (bxz.size() == 0) continue;
                
                Map<VectorXd> ei_bxz(&bxz[0],bxz.size());
                Map<VectorXd> ei_sexz(&sexz[0],sexz.size());
                
                zsxz=ei_bxz.array()/ei_sexz.array();
                if(!heidiFlag) maxid=max_abs_id(zsxz);
                double pxz_val = pchisq(zsxz[maxid]*zsxz[maxid], 1);
                
                
                if(!heidiFlag && pxz_val>p_smr) continue;
                else outCount++;
                
                double bxy_val = byz[maxid] / bxz[maxid];
                double sexy_val = sqrt((seyz[maxid] * seyz[maxid] * bxz[maxid] * bxz[maxid] + sexz[maxid] * sexz[maxid] * byz[maxid] * byz[maxid]) / (bxz[maxid] * bxz[maxid] * bxz[maxid] * bxz[maxid]));
                double chisqxy = bxy_val*bxy_val / (sexy_val*sexy_val);
                
                
                double pxy_val = pchisq(chisqxy, 1);  //   double pxy=chi_prob(1,chisqxy); //works
                
                double chisqyz = byz[maxid] / seyz[maxid];
                double pyz_val = pchisq(chisqyz*chisqyz, 1);
                
                out_probid[outCount]= i;
                bxy[outCount]=dtosf(bxy_val);
                sexy[outCount]=dtosf(sexy_val);
                pxy[outCount]=dtos(pxy_val);
                bgwas[outCount]=dtosf(byz[maxid]);
                segwas[outCount]=dtosf(seyz[maxid]);
                beqtl[outCount]=dtosf(bxz[maxid]);
                seeqtl[outCount]=dtosf(sexz[maxid]);
                pgwas[outCount]=dtos(pyz_val);
                peqtl[outCount]=dtos(pxz_val);
                rsid[outCount]=eName[maxid];
                rschr[outCount]=atos(snpchrom[maxid]);
                rsbp[outCount]=itos(bpsnp[maxid]);
                if(heidioffFlag)   rsfreq[outCount]=string("NA");
                else rsfreq[outCount]=dtosf(freq[maxid]);
                
                rsa1[outCount]=allele1[maxid];
                rsa2[outCount]=allele2[maxid];
                
                if(heidioffFlag)
                {
                    prb1[outCount]= string("NA");
                    nsnp_test1[outCount]= string("NA");
                }
                else
                {
                    //extract info from reference
                    make_XMat(&bdata,curId, _X); //_X: one row one individual, one column one SNP
                    //last vesion ref_snpData was used. row of ref_snpData is SNP, column of ref_snpData is individual
                    //cor_calc(_LD, _X);
                    
                    ld_calc_o2m(ld_v,maxid,_X);
                    
                    //test here
                    // for( int kk=0;kk<curId.size();kk++) if(abs(ld_v[kk]-_LD.col(maxid)[kk])>1e-6) cout<<"wrong"<<endl;
                    // test end
                    
                    sn_ids.clear(); //increase order
                    if(abs(ld_top-1)<1e-6) get_square_idxes(sn_ids,zsxz,threshold);
                    //else get_square_ldpruning_idxes(sn_ids,zsxz,threshold,_LD, maxid,ld_top);
                    else get_square_ldpruning_idxes(sn_ids,zsxz,threshold,ld_v, maxid,ld_top);
                    
                    if(sn_ids.size() < m_hetero)
                    {
                        prb1[outCount]= string("NA");
                        nsnp_test1[outCount]= string("NA");
                        top_match1[outCount]= string("NA");
                        ldrsq[outCount]= string("NA");
                        continue;
                    }
                    
                    if(plotflg){
                        for(long j=strlen(outFileName)-1;j>=0;j--)
                            if(outFileName[j]=='/')
                            {
                                plotdir=string(outFileName).substr(0,j+1);
                                break;
                            }
                        if(plotdir=="") plotdir="./";
                        
                        plotdir=string(plotdir)+"plot";
                        struct stat st = {0};
                        if (stat(plotdir.c_str(), &st) == -1) {
                            mkdir(plotdir.c_str(), 0755);
                        }
                        
                        plotdir=string(plotdir)+"/"+traitname;
                        st = {0};
                        if (stat(plotdir.c_str(), &st) == -1) {
                            mkdir(plotdir.c_str(), 0755);
                        }
                        
                        plotdir=string(plotdir)+"/"+atos(plotdir_id);
                        st = {0};
                        if (stat(plotdir.c_str(), &st) == -1) {
                            mkdir(plotdir.c_str(), 0755);
                        }
                        
                        plot_paths.push_back(string(plotdir)+"/"+esdata._epi_prbID[i]);
                    }
                    

                    if(plotflg){
                        for(int j=0;j<sn_ids.size();j++) {
                            plot_probeidx.push_back(i);
                            plot_snpidx.push_back(curId[sn_ids[j]]);
                            plot_bxz.push_back(bxz[sn_ids[j]]);
                            plot_sexz.push_back(sexz[sn_ids[j]]);
                        }
                        
                        string ldfile = string(plotdir)+"/"+esdata._epi_prbID[i]+".engs";
                        ofstream ldio(ldfile.c_str());
                        if (!ldio) throw ("Error: can not open the file " + ldfile + " to save!");
                        ldio << "Expo_Chr" <<'\t' << "Expo_ID"  << '\t' << "Expo_bp" << '\t'<< "SNP_Chr"<< '\t'<< "SNP"<< '\t'  << "SNP_bp"<< '\t'<< "A1"<< '\t'<< "A2"<< '\t'<<"b_Outco"<<'\t'<<"se_Outco"<<'\t'<<"b_Expo"<<'\t'<<"se_Expo"<<'\n';
                        for (int ii = 0;ii <plot_snpidx.size(); ii++) {
                            ldio<<esdata._epi_chr[plot_probeidx[ii]]<<'\t'<<esdata._epi_prbID[plot_probeidx[ii]]<<'\t'<<esdata._epi_bp[plot_probeidx[ii]]<<'\t'<<esdata._esi_chr[plot_snpidx[ii]]<<'\t'<<esdata._esi_rs[plot_snpidx[ii]]<<'\t'<<esdata._esi_bp[plot_snpidx[ii]]<<'\t'<<esdata._esi_allele1[plot_snpidx[ii]]<<'\t'<<esdata._esi_allele2[plot_snpidx[ii]]<<'\t'<<gdata.byz[plot_snpidx[ii]]<<'\t'<<gdata.seyz[plot_snpidx[ii]]<<'\t'<<plot_bxz[ii]<<'\t'<<plot_sexz[ii]<<'\n';
                        }
                        cout<<"informantion of "<<plot_snpidx.size()<<" probes for plot have been saved in the file [" + ldfile + "]."<<endl;
                        ldio.close();
                        

                    }
                    _byz.resize(sn_ids.size());
                    _seyz.resize(sn_ids.size());
                    _bxz.resize(sn_ids.size());
                    _sexz.resize(sn_ids.size());
                    _zsxz.resize(sn_ids.size());
                    // _LD_heidi.resize(sn_ids.size(),sn_ids.size());
                    _X_heidi.resize(_X.rows(), sn_ids.size());
                    
                    #pragma omp parallel for
                    for(int j=0;j<sn_ids.size();j++)
                    {
                        _byz[j]=byz[sn_ids[j]];
                        _seyz[j]=seyz[sn_ids[j]];
                        _bxz[j]=bxz[sn_ids[j]];
                        _sexz[j]=sexz[sn_ids[j]];
                        _zsxz[j]=zsxz[sn_ids[j]];
                        //   for(int k=0;k<=j;k++)_LD_heidi(j,k)=_LD_heidi(k,j)=_LD(sn_ids[j],sn_ids[k]);
                        _X_heidi.col(j)=_X.col(sn_ids[j]);
                    }
                    _X.resize(0,0);
                    cor_calc(_LD_heidi, _X_heidi);
                    
                    if(plotflg)
                    {
                        string ldidfile = string(plotdir)+"/"+esdata._epi_prbID[i]+".ld.id";
                        ofstream ldio(ldidfile.c_str());
                        if (!ldio) throw ("Error: can not open the file " + ldidfile + " to save!");
                        for (int ii = 0;ii <plot_snpidx.size(); ii++)
                            ldio<<esdata._esi_rs[plot_snpidx[ii]]<<'\n';
                        cout<<"rs id of "<<plot_snpidx.size()<<" unique SNPs for plot have been saved in the file [" + ldidfile + "]."<<endl;
                        ldio.close();
                        
                        string ldinfofile = string(plotdir)+"/"+esdata._epi_prbID[i]+".ld.gz";
                        gzFile gz_outfile = gzopen(ldinfofile.c_str(), "wb");
                        string ldstrs="";
                        for(int ii=0;ii<_LD_heidi.cols();ii++)
                            for(int jj=0;jj<ii;jj++) ldstrs+=atos(_LD_heidi(ii,jj))+"\n";
                        
                        if(gzputs(gz_outfile, ldstrs.c_str()) == -1) cout<<"error"<<endl;
                        gzclose(gz_outfile);
                        cout<< "Lower triangle of LD score matrix of "<<plot_snpidx.size()<<" SNPs have been saved by row-major in the file ["+ ldinfofile + "]."<<endl;
                    }

                    
                    _X_heidi.resize(0,0);
                    
                    long nsnp = sn_ids.size();
                    double pdev=bxy_hetero3(_byz,  _bxz, _seyz, _sexz, _zsxz, _LD_heidi, &nsnp);
                    
                    prb1[outCount] = dtos(pdev);
                    if(nsnp>0) nsnp_test1[outCount] = itos(nsnp);
                    else nsnp_test1[outCount] = string("NA");
                    
                    
                    // top GWAS SNP ?= top eQTL
                    int indx1 = max_abs_id(_zsxz);
                    _byz = _byz.array() / _seyz.array();
                    int indx2 = max_abs_id(_byz);
                    
                    if(indx1 == indx2) top_match1[outCount] =itos(1);
                    else top_match1[outCount] = itos(0);
                    double ldrsqVal = _LD_heidi(indx1, indx2) * _LD_heidi(indx1, indx2);
                    ldrsq[outCount]=dtosf(ldrsqVal);
                }
                
            }
            
            //genesn in R is just probidx here. can refer to probinfo to get probeid, probenm, chr, gene, bp
            if(plotflg && !heidioffFlag)
            {
                string ldfile = string(outFileName)+"."+traitname+".plotpath.txt";
                ofstream ldio(ldfile.c_str());
                if (!ldio) throw ("Error: can not open the file " + ldfile + " to save!");
                
                for (int i = 0;i <plot_paths.size(); i++) {
                    ldio<<plot_paths[i]<<'\n';
                }
                cout<<"informantion of "<<plot_paths.size()<<" probes for plot have been saved in the file [" + ldfile + "]."<<endl;
                ldio.close();

            }
            if(outCount>=0)
            {
                string smrfile = string(outFileName)+"."+traitname+".smr";
                ofstream smr(smrfile.c_str());
                if (!smr) throw ("Error: can not open the file " + smrfile + " to save!");
                
                smr << "Expo_ID" <<'\t'<< "Expo_Chr" <<'\t' << "Expo_Gene"  << '\t' << "Expo_bp" << '\t'<< "Outco_ID" <<'\t'<< "Outco_Chr" <<'\t' << "Outco_Gene"  << '\t' << "Outco_bp" << '\t'<< "SNP"<< '\t' << "SNP_Chr"<< '\t' << "SNP_bp"<< '\t' << "A1"<< '\t'<< "A2"<< '\t'<<"Freq"<<'\t'<<"b_Outco"<<'\t'<<"se_Outco"<<'\t'<< "p_Outco" << '\t'<<"b_Expo"<<'\t'<<"se_Expo"<<'\t'<< "p_Expo" << '\t'<< "b_SMR" << '\t'<< "se_SMR"<< '\t' << "p_SMR" << "\t"<< "p_HET"<< "\t" << "nsnp" << '\n';
                
                for (int i = 0;i <=outCount; i++) {
                    smr<<esdata._epi_prbID[out_probid[i]]<<'\t'<<esdata._epi_chr[out_probid[i]]<<'\t'<<esdata._epi_gene[out_probid[i]]<<'\t'<<esdata._epi_bp[out_probid[i]]<<'\t'<<traitname<<'\t'<<etrait._epi_chr[ii]<<'\t'<<etrait._epi_gene[ii]<<'\t'<<etrait._epi_bp[ii]<<'\t'<<rsid[i]<<'\t'<<rschr[i]<<'\t'<<rsbp[i]<<'\t'<<rsa1[i]<<'\t'<<rsa2[i]<<'\t'<<rsfreq[i]<<'\t'<<bgwas[i]<<'\t'<<segwas[i]<<'\t'<<pgwas[i]<<'\t'<<beqtl[i]<<'\t'<<seeqtl[i]<<'\t'<<peqtl[i]<<'\t'<<bxy[i]<<'\t'<<sexy[i]<<'\t'<<pxy[i]<<'\t'<<prb1[i]<<'\t'<<nsnp_test1[i]<<'\n';
                }
                cout<<"SMR and heterogeneity analysis finished.\nSMR and heterogeneity analysis results of "<<outCount+1<<" probes have been saved in the file [" + smrfile + "]."<<endl;
                smr.close();

            }else cout<<"SMR and heterogeneity analysis finished.\nSMR and heterogeneity analysis results of "<<outCount+1<<" probes have been saved."<<endl;
           
       
            free_gwas_data( &gdata);
            
        }
    }
    
    int read_frqfile(eqtlInfo* eqtlinfo, string frqfile)
    {
        int sample_size=0;
        ifstream frq(frqfile.c_str());
        if (!frq) throw ("Error: can not open the file [" + frqfile + "] to read.");
        cout << "Reading eQTL probe information from [" + frqfile + "]." << endl;
        eqtlinfo->_esi_maf.clear();
        
        char buf[MAX_LINE_SIZE];
        int lineNum(0);
        while(!frq.eof())
        {
            frq.getline(buf,MAX_LINE_SIZE);
            lineNum++;
        }
        if(buf[0]=='\0') lineNum--;
        
        cout << lineNum << " SNPs to be included from [" + frqfile + "]." << endl;
        
        eqtlinfo->_esi_maf.resize(eqtlinfo->_esi_rs.size());
        vector<string> tmp_rs;
        vector<float> tmp_maf;
        frq.clear(ios::goodbit);
        frq.seekg (0, ios::beg);
        for(int i=0;i<lineNum;i++)
        {
            string tmpStr;
            frq.getline(buf,MAX_LINE_SIZE);
            istringstream iss(buf);
            iss>>tmpStr;
            iss>>tmpStr;
            tmp_rs.push_back(tmpStr.c_str());
            iss>>tmpStr;
            iss>>tmpStr;
            iss>>tmpStr;
            tmp_maf.push_back(atof(tmpStr.c_str()));
            iss>>tmpStr;
            sample_size=atoi(tmpStr.c_str());
        }
        frq.close();
        
        vector<int> idx;
        match_only(eqtlinfo->_esi_rs, tmp_rs, idx);
        if(idx.size()!=eqtlinfo->_esi_rs.size())
        {
            cout<<"Some SNPs in summary data are not in genotype data!"<<endl;
            exit(1);
        }
        for(int i=0;i<idx.size();i++)
        {
            eqtlinfo->_esi_maf[i]=tmp_maf[idx[i]];
        }
        
        return sample_size>>1;
    }
    void write_besd(string outFileName, eqtlInfo* eqtlinfo)
    {
        filter_probe_null(eqtlinfo); // at the same time, reset the vector _include
        cout<<"\nsaving eQTL data..."<<endl;
        string esdfile = string(outFileName)+".esi";
        ofstream smr(esdfile.c_str());
        if (!smr) throw ("Error: can not open the esi file " + esdfile + " to save!");
        for (int i = 0;i <eqtlinfo->_snpNum; i++) {
            smr<<eqtlinfo->_esi_chr[i]<<'\t'<<eqtlinfo->_esi_rs[i]<<'\t'<<eqtlinfo->_esi_gd[i]<<'\t'<<eqtlinfo->_esi_bp[i]<<'\t'<<eqtlinfo->_esi_allele1[i]<<'\t'<<eqtlinfo->_esi_allele2[i]<<'\n';
        }
        smr.close();
        cout<<eqtlinfo->_snpNum<<" SNPs have been saved in the file [" + esdfile + "]."<<endl;
        
        esdfile = string(outFileName)+".epi";
        smr.open(esdfile.c_str());
        if (!smr) throw ("Error: can not open the epi file " + esdfile + " to save!");
        for (int i = 0;i <eqtlinfo->_include.size(); i++) {
            smr<<eqtlinfo->_epi_chr[eqtlinfo->_include[i]]<<'\t'<<eqtlinfo->_epi_prbID[eqtlinfo->_include[i]]<<'\t'<<eqtlinfo->_epi_gd[eqtlinfo->_include[i]]<<'\t'<<eqtlinfo->_epi_bp[eqtlinfo->_include[i]]<<'\t'<<eqtlinfo->_epi_gene[eqtlinfo->_include[i]]<<'\t'<<eqtlinfo->_epi_orien[eqtlinfo->_include[i]]<<'\n';
        }
        smr.close();
        cout<<eqtlinfo->_include.size()<<" probes have been saved in the file [" + esdfile + "]."<<endl;
        
        esdfile = string(outFileName)+".besd";
        FILE * smrbesd;
        smrbesd = fopen (esdfile.c_str(), "wb");
        if(eqtlinfo->_valNum==0)
        {
            uint64_t bsize=(eqtlinfo->_include.size()*eqtlinfo->_snpNum<<1)+1;
            float* buffer=(float*)malloc (sizeof(float)*bsize);
            memset(buffer,0,sizeof(float)*bsize);
            float* ptr=buffer;
            *ptr++=0.0;
            uint64_t pro_num=eqtlinfo->_include.size();
            uint64_t snp_num=eqtlinfo->_snpNum;
            for(int i=0;i<pro_num;i++)
            {
                memcpy(ptr+(i<<1)*snp_num,&eqtlinfo->_bxz[eqtlinfo->_include[i]][0],sizeof(float)*snp_num);
                memcpy(ptr+((i<<1)+1)*snp_num,&eqtlinfo->_sexz[eqtlinfo->_include[i]][0],sizeof(float)*snp_num);
            }
            fwrite(buffer,sizeof(float), bsize, smrbesd);
            free(buffer);
        }
        else
        {
            uint64_t colSize=sizeof(uint64_t)*((eqtlinfo->_include.size()<<1)+1);
            uint64_t rowSize=sizeof(uint32_t)*eqtlinfo->_valNum;
            uint64_t valSize=sizeof(float)*eqtlinfo->_valNum;
            uint64_t valNum=eqtlinfo->_valNum;
            uint64_t bufsize=sizeof(float)+sizeof(uint64_t)+colSize+rowSize+valSize;
            
            char* buffer=(char*)malloc (sizeof(char)*bufsize);
            memset(buffer,0,sizeof(char)*bufsize);
            float ftype=SPARSE_FILE_TYPE_3;
            memcpy(buffer,&ftype,sizeof(float));
            char* wptr=buffer+sizeof(float);
            memcpy(wptr,&valNum,sizeof(uint64_t));
            wptr+=sizeof(uint64_t);
            uint64_t* uptr=(uint64_t*)wptr; *uptr++=0;
            for(int i=0;i<eqtlinfo->_include.size();i++)
            {
                *uptr++=eqtlinfo->_cols[(eqtlinfo->_include[i]<<1)+1];
                *uptr++=eqtlinfo->_cols[eqtlinfo->_include[i]+1<<1];
            }
            wptr+=colSize;
            memcpy(wptr,&eqtlinfo->_rowid[0],rowSize);
            wptr+=rowSize;
            memcpy(wptr,&eqtlinfo->_val[0],valSize);
            fwrite (buffer,sizeof(char), bufsize, smrbesd);
            free(buffer);

        }

        
        fclose (smrbesd);
        
        cout<<"Beta values and SE values for "<<eqtlinfo->_include.size()<<" Probes and "<<eqtlinfo->_snpNum<<" SNPs have been saved in the binary file [" + esdfile + "]." <<endl;
        

    }
    
    void standardization(char* outFileName, char* eqtlFileName,bool bFlag,char* freqName)
    {
        eqtlInfo esdata;
        if(eqtlFileName==NULL) throw("Error: please input eQTL summary data for SMR analysis by the flag --eqtl-summary.");
        if(freqName==NULL) throw("Error: please input feq data for SMR analysis by the flag --freq.");
        read_esifile(&esdata, string(eqtlFileName)+".esi");
        read_epifile(&esdata, string(eqtlFileName)+".epi");
        if(bFlag) read_besdfile(&esdata, string(eqtlFileName)+".besd");
        else      read_esdfile(&esdata, string(eqtlFileName)+".esd");
        
        int n=read_frqfile(&esdata, string(freqName));
        for(int i=0;i<esdata._probNum;i++)
        {
            if(esdata._rowid.empty())
            {
                for (int j = 0; j<esdata._esi_include.size(); j++)
                {
                    if (abs(esdata._bxz[i][j] + 9) > 1e-6)
                    {
                        
                        float bxz=esdata._bxz[i][j];
                        float sexz=esdata._sexz[i][j];
                        float p=esdata._esi_maf[j];
                        float z=bxz/sexz;
                        float b=z/sqrt(2*p*(1-p)*(n+z*z));
                        float se=1/sqrt(2*p*(1-p)*(n+z*z));
                        esdata._bxz[i][j]=b;
                        esdata._sexz[i][j]=se;
                    }
                }
                
            }
            else{
                uint64_t beta_start=esdata._cols[i<<1];
                uint64_t se_start=esdata._cols[1+(i<<1)];
                uint64_t numsnps=se_start-beta_start;
                for(uint64_t j=0;j<numsnps;j++)
                {
                    uint64_t ge_rowid=esdata._rowid[beta_start+j];
                    float bxz=esdata._val[beta_start+j];
                    float sexz=esdata._val[se_start+j];
                    float p=esdata._esi_maf[ge_rowid];
                    float z=bxz/sexz;
                    float b=z/sqrt(2*p*(1-p)*(n+z*z));
                    float se=1/sqrt(2*p*(1-p)*(n+z*z));
                    esdata._val[beta_start+j]=b;
                    esdata._val[se_start+j]=se;
                }
            }
            
        }
        write_besd(outFileName, &esdata);
    }

    void meta(char* outFileName,char* eqtlFileName, char* eqtlFileName2)
    {
        setNbThreads(thread_num);
        
        eqtlInfo etrait;
        eqtlInfo esdata;
        eqtlInfo metadata;
        
        if(eqtlFileName==NULL) throw("Error: please input eQTL summary data for SMR analysis by the flag --eqtl-summary.");
        if(eqtlFileName2==NULL) throw("Error: please input another eQTL summary data for SMR analysis by the flag --eqtl-summary.");
        
        read_esifile(&etrait, string(eqtlFileName)+".esi");
        read_esifile(&esdata, string(eqtlFileName2)+".esi");
        vector<int> idx;
        vector<string> cmmnSymbs;
        match_only(esdata._esi_rs, etrait._esi_rs, idx);
        etrait._esi_include=idx;
        for(int i=0;i<idx.size();i++) cmmnSymbs.push_back(etrait._esi_rs[idx[i]]);
        idx.clear();
        match_only(cmmnSymbs,esdata._esi_rs,idx);
        esdata._esi_include=idx;
        cout<<"There are "<<idx.size()<<" SNPs in common."<<endl;
        read_epifile(&etrait, string(eqtlFileName)+".epi");
        read_epifile(&esdata, string(eqtlFileName2)+".epi");
        idx.clear();
        match_only(esdata._epi_prbID, etrait._epi_prbID, idx);
        etrait._include=idx;
        cmmnSymbs.clear();
        for(int i=0;i<idx.size();i++) cmmnSymbs.push_back(etrait._epi_prbID[idx[i]]);
        idx.clear();
        match_only(cmmnSymbs,esdata._epi_prbID,idx);
        esdata._include=idx;
        cout<<"There are "<<idx.size()<<" Probes in common."<<endl;
        read_besdfile(&etrait, string(eqtlFileName)+".besd");
        read_besdfile(&esdata, string(eqtlFileName2)+".besd");
        
        metadata._cols.push_back(0);
        cout<<"Performing Meta analysis..."<<endl;
        map<string, int> unmatched_rs_map;
        int unmatched_map_size=0;
        for(int i=0;i<etrait._probNum;i++)
        {
            string ref_pid=etrait._epi_prbID[i];
            string alt_pid=esdata._epi_prbID[i];
            if(ref_pid!=alt_pid)
            {
                cout<<"Some bugs here, please help to report!"<<endl;
                exit(1);
            }
            
            vector<float> ref_byz;
            vector<float> ref_seyz;
            vector<char> ref_a1;
            vector<char> ref_a2;
            vector<uint32_t> ref_rowid;
            vector<float> meta_beta;
            vector<float> meta_se;
            vector<uint32_t> meta_rowid;
            
            uint64_t beta_start=etrait._cols[i<<1];
            uint64_t se_start=etrait._cols[1+(i<<1)];
            uint64_t numsnps=se_start-beta_start;
            for(uint64_t j=0;j<numsnps;j++)
            {
                uint32_t ge_rowid=etrait._rowid[beta_start+j];
                ref_rowid.push_back(ge_rowid);
                ref_a1.push_back(etrait._esi_allele1[ge_rowid]);
                ref_a2.push_back(etrait._esi_allele2[ge_rowid]);
                ref_byz.push_back(etrait._val[beta_start+j]);
                ref_seyz.push_back(etrait._val[se_start+j]);
            }
            
            vector<float> byz;
            vector<float> seyz;
            vector<char> a1;
            vector<char> a2;
            vector<uint32_t> rowid;
          
            
            uint64_t beta_stt=esdata._cols[i<<1];
            uint64_t se_stt=esdata._cols[1+(i<<1)];
            uint64_t nums=se_stt-beta_stt;
            for(uint64_t j=0;j<nums;j++)
            {
                uint32_t ge_rid=esdata._rowid[beta_stt+j];
                rowid.push_back(ge_rid);
                a1.push_back(esdata._esi_allele1[ge_rid]);
                a2.push_back(esdata._esi_allele2[ge_rid]);
                byz.push_back(esdata._val[beta_stt+j]);
                seyz.push_back(esdata._val[se_stt+j]);
               
            }
            
            vector<int> idx;
            match(ref_rowid,rowid,idx);
            for(int j=0;j<idx.size();j++)
            {
                if(idx[j]==-9) continue;
                uint32_t tmp_rowid=ref_rowid[j];
                string tmp_rs=etrait._esi_rs[tmp_rowid];
                if(tmp_rowid!=rowid[idx[j]])
                {
                    cout<<"Some bugs here, please help to report!"<<endl;
                    exit(1);
                }
                
                char tmp_ref_a1=ref_a1[j];
                char tmp_ref_a2=ref_a2[j];
                char tmp_alt_a1=a1[idx[j]];
                char tmp_alt_a2=a2[idx[j]];
                float se1=ref_seyz[j];
                float se2=seyz[idx[j]];
                float beta1=ref_byz[j];
                float beta2=byz[idx[j]];
                
                
                if (tmp_ref_a1 == tmp_alt_a1 && tmp_ref_a2 == tmp_alt_a2)
                {
                    float tmpSE=se1*se2/sqrt(se1*se1+se2*se2);
                    meta_rowid.push_back(tmp_rowid);
                    meta_se.push_back(tmpSE);
                    meta_beta.push_back((beta1/(se1*se1) + beta2/(se2*se2))*tmpSE*tmpSE);
                }
                else if(tmp_ref_a1 == tmp_alt_a2 && tmp_ref_a2 == tmp_alt_a1)
                {
                    beta2=-beta2;
                    float tmpSE=se1*se2/sqrt(se1*se1+se2*se2);
                    meta_rowid.push_back(tmp_rowid);
                    meta_se.push_back(tmpSE);
                    meta_beta.push_back((beta1/(se1*se1) + beta2/(se2*se2))*tmpSE*tmpSE);
                }
                else {
                    unmatched_rs_map.insert(pair<string, int>(tmp_rs, unmatched_map_size));
                   
                }
            }
        
            for(int j=0;j<meta_beta.size();j++) metadata._val.push_back(meta_beta[j]);
            for(int j=0;j<meta_beta.size();j++) metadata._val.push_back(meta_se[j]);
            for(int j=0;j<meta_beta.size();j++) metadata._rowid.push_back(meta_rowid[j]);
            for(int j=0;j<meta_beta.size();j++) metadata._rowid.push_back(meta_rowid[j]);
            metadata._cols.push_back(meta_beta.size()+metadata._cols[metadata._cols.size()-1]);
            metadata._cols.push_back(meta_beta.size()+metadata._cols[metadata._cols.size()-1]);
           
        }
        metadata._esi_allele1=etrait._esi_allele1;
        metadata._esi_allele2=etrait._esi_allele2;
        metadata._esi_bp=etrait._esi_bp;
        metadata._esi_chr=etrait._esi_chr;
        metadata._esi_gd=etrait._esi_gd;
        metadata._esi_include=etrait._esi_include;
        metadata._esi_rs=etrait._esi_rs;
        metadata._epi_bp=etrait._epi_bp;
        metadata._epi_chr=etrait._epi_chr;
        metadata._epi_gd=etrait._epi_gd;
        metadata._epi_gene=etrait._epi_gene;
        metadata._epi_orien=etrait._epi_orien;
        metadata._epi_prbID=etrait._epi_prbID;
        metadata._include=etrait._include;
        metadata._probNum=etrait._probNum;
        metadata._snpNum=etrait._snpNum;
        metadata._valNum=metadata._val.size();
        
        string unmatchedsnpfname = string(outFileName)+".unmatched.snp.list";
        FILE* unmatchedsnpfile=fopen(unmatchedsnpfname.c_str(), "w");
        if (!(unmatchedsnpfile)) {
            printf("Error: Failed to open unmatchedsnpfile file.\n");
            exit(1);
        }
        for(std::map<string,int>::iterator it=unmatched_rs_map.begin(); it!=unmatched_rs_map.end(); ++it){
            string tmpstr=it->first+'\n';
            fputs(tmpstr.c_str(),unmatchedsnpfile);
        }
        fclose(unmatchedsnpfile);
        write_besd(outFileName, &metadata);
        
    }
}
