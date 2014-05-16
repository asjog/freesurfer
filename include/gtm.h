/**
 * @file  gtm.h
 * @brief Routines to create and analyze the Geometric Transfer Matrix (GTM)
 *
 * REPLACE_WITH_LONG_DESCRIPTION_OR_REFERENCE
 */
/*
 * Original Author: Douglas N. Greve
 * CVS Revision Info:
 *    $Author: greve $
 *    $Date: 2014/05/13 21:41:56 $
 *    $Revision: 1.5 $
 *
 * Copyright © 2011 The General Hospital Corporation (Boston, MA) "MGH"
 *
 * Terms and conditions for use, reproduction, distribution and contribution
 * are found in the 'FreeSurfer Software License Agreement' contained
 * in the file 'LICENSE' found in the FreeSurfer distribution, and here:
 *
 * https://surfer.nmr.mgh.harvard.edu/fswiki/FreeSurferSoftwareLicense
 *
 * Reporting: freesurfer@nmr.mgh.harvard.edu
 *
 */


#ifndef GTM_INC
#define GTM_INC

#include "matrix.h"
#include "mri.h"
#include "transform.h"
#include "mrisurf.h"

#ifdef X
#undef X
#endif

typedef struct
{
  char *subject;
  int USF;
  char *apasfile;
  char *ctxannotfile;
  int ctxlhbase,ctxrhbase;
  MRIS *lhw, *lhp, *rhw, *rhp;
  int KeepHypo;
  int KeepCC;
  int SubSegWM;
  char *wmannotfile;
  int wmlhbase,wmrhbase;
  float dmax;
  int nlist,srclist[300],targlist[300];
  MRI *seg;
  LTA *anat2seg;
  int lhmin, lhmax, rhmin, rhmax;
} GTMSEG;
typedef struct 
{
  char *name; // contrast name
  MATRIX *C;
  MATRIX *gamma;
  MATRIX *gammavar;
  MATRIX *t;
  MATRIX *p;
} GTM_CONTRAST, GTMCON;


typedef struct 
{
  MRI *yvol; // source (PET) data
  MRI *anatseg; // segmentation in anatomical space, may be hires (ie, not conformed)
  LTA *anat2pet; // registration from anat to pet space
  LTA *anat2seg; // registration from FS conformed space to seg
  LTA *seg2anat; // inverse registration
  LTA *seg2pet;  // registration from segmentation to source volume
  MRI *mask;     // binary mask in PET space
  double cFWHM, rFWHM, sFWHM; // assumed FWHM of PSF
  double cStd, rStd, sStd; // PSF FWHM converted to standard dev

  double automask_fwhm,automask_thresh; // Use this FWHM instead of PSF when computing mask
  int automask_reduce_fov; // Flag: reduce PET FoV to be tight to mask.
  MRI_REGION *automaskRegion; // Keep reduction region in case reinstate at original FoV
  MRI *yvol_full_fov; // Used to keep header of original FoV source data
  double PadThresh; // Used to dilate mask based on automask_fwhm
  int nPad; // Actual amount of dilation

  int rescale,n_scale_refids,scale_refids[100];
  double scale;

  MRI *segpvf; // PVF of each seg (one in each frame)
  MRI *ttpvf; // PVF of each tissue type (one in each frame), used by MG
  MRI *gtmseg; // Segmentation in PET space
  int nmask; // number of voxels in the mask

  int nsegs,*segidlist; // number of segments in segmentation, list of segids
  int *nperseg; // number of voxels per seg
  COLOR_TABLE *ctGTMSeg; // color table of segments
  int nReplace , SrcReplace[1000], TrgReplace[1000]; // for replacing segs
  MATRIX *nvox; // same as nperseg, doh
  MATRIX *vrf;  // variance reduction factor for each seg
  MATRIX *segrvar; // residual variance in each seg

  // GLM stuff for GTM
  MATRIX *X,*X0;
  MATRIX *y,*Xt, *XtX, *iXtX, *Xty, *beta, *res, *yhat,*betavar;
  MATRIX *rvar,*rvargm; // residual variance, all vox and only GM
  int dof;
  double XtXcond;
  MRI *ysynth,*ysynthsm; // synthesized vs yhat
  MATRIX *skew,*kurtosis; // for QAe
  int DoVoxFracCor; // Flag to correct for volume fraction effect

  int nContrasts;
  GTMCON *contrasts[100];

  MRI *rbv; // RBV computed volume
  int mask_rbv_to_brain; // Reduce FoV of RBV to be tight to brain
  MRI *yseg; // source volume trilin resampled to seg space (used with RBV)
  MRI *yhat0seg; // unsmoothed yhat created in seg space (used with RBV)
  MRI *yhatseg;  // smoothed yhat in seg space (used with RBV)
  MRI *rbvsegmean; // seg mean in RBV, used for QA

  int DoMGPVC; // Muller-Gartner
  MRI *mg; // MG output volume
  double mg_gmthresh; // GM PVF threshold 
  int n_mg_refids,mg_refids[100]; // WM reference seg IDs
  MATRIX *mg_reftac; // WM reference TAC

  int DoKMRef; // Kinetic Modeling Reference TAC
  int n_km_refids,km_refids[100]; // KM reference seg IDs
  MATRIX *km_reftac; // KM reference TAC

  int DoKMHB; // Kinetic Modeling HiBinding TAC
  int n_km_hbids,km_hbids[100];// KM HiBinding seg IDs
  MATRIX *km_hbtac; // KM HiBinding TAC

  char *OutDir; // output folder
  FILE *logfp;  // log file pointer
} GTM;


int MRIgtmSeg(GTMSEG *gtmseg);
int GTMSEGprint(GTMSEG *gtmseg, FILE *fp);
int GTMdefaultSegReplacmentList(int *nReplace, int *ReplaceThis, int *WithThat);
COLOR_TABLE *GTMSEGctab(GTMSEG *gtmseg, COLOR_TABLE *ctSubCort);

GTM *GTMalloc();
int GTMfree(GTM **pGTM);
int GTMmatrixY(GTM *gtm);
int GTMsetNMask(GTM *gtm);
int GTMpsfStd(GTM *gtm);
int GTMsegidlist(GTM *gtm);
int GTMnPad(GTM *gtm);
int GTMbuildX(GTM *gtm);
int GTMsolve(GTM *gtm);
int GTMsegrvar(GTM *gtm);
int GTMsynth(GTM *gtm);
int GTMsmoothSynth(GTM *gtm);
MRI *GTMsegSynth(GTM *gtm);
int GTMrbv(GTM *gtm);
int GTMmgpvc(GTM *gtm);
MATRIX *GTMvol2mat(GTM *gtm, MRI *vol, MATRIX *m);
MRI *GTMmat2vol(GTM *gtm, MATRIX *m, MRI *vol);
int GTMprintReplaceList(FILE *fp, const int nReplace, const int *ReplaceThis, const int *WithThat);
int GTMcheckReplaceList(const int nReplace, const int *ReplaceThis, const int *WithThat);
int GTMloadReplacmentList(const char *fname, int *nReplace, int *ReplaceThis, int *WithThat);
int GTMcheckX(MATRIX *X);
int GTMautoMask(GTM *gtm);
int GTMrvarGM(GTM *gtm);
int GTMttest(GTM *gtm);
int GTMmgRefIds(GTM *gtm);
int GTMprintMGRefTAC(GTM *gtm, FILE *fp);
int GTMwriteMGRefTAC(GTM *gtm, char *filename);
int GTMrescale(GTM *gtm);
int GTMwriteContrasts(GTM *GTM);
int GTMprintRefIds(GTM *gtm, FILE *fp);
int GTMcheckRefIds(GTM *gtm);
int GTMrefTAC(GTM *gtm);
int VRFStats(GTM *gtm, double *vrfmean, double *vrfmin, double *vrfmax);
int WriteVRFStats(char *fname, GTM *gtm);

#endif