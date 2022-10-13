#!/bin/sh

cd $TMPDIR
mkdir Job_$1_$2
echo "============ Created the master run folder ============"

cd Job_$1_$2
cd /user/asahasra/CMSSW_12_4_9/src/
eval `scramv1 runtime -sh`
cd $TMPDIR/Job_$1_$2
pwd

cp /user/asahasra/CMSSW_12_4_9/src/EDMGenAnalyzer/EDMGenAnalyzerJPsiToEE/test/JPsiToEEGenAnalyzer.cc ./
cp /user/asahasra/CMSSW_12_4_9/src/EDMGenAnalyzer/EDMGenAnalyzerJPsiToEE/test/JPsiToEEHepMCAnalysis_cfg.py ./

echo "============ Copied the cpp run files file. Listing directory contents ============"
ls
echo "======================= End listing the directory contents ========================"
echo ""

echo "Begin the execution loop"

for fNum in $(ls /pnfs/iihe/cms/store/user/asahasra/JPsiToEE_Pt2To30_13p6TeV_TuneCP5_pythia8/PrivateMC_CMSSW1245_220823GEN/220823_152058/0000/JPsiToEE_13p6_500_GEN_*.root  | grep -Po '500_GEN_[0-9]+' | sed 's/500_GEN_[^0-9]*//')
do
    echo "==========Checking for file $fNum: In job $1, for process $2 in total $3=========="
    if [ $((fNum%$3)) -eq $2 ]
    then
	echo "Condition true for file $fNum: In job $1, for process $2 in total $3"
	cmsRun JPsiToEEHepMCAnalysis_cfg.py inputFileName="file:/pnfs/iihe/cms/store/user/asahasra/JPsiToEE_Pt2To30_13p6TeV_TuneCP5_pythia8/PrivateMC_CMSSW1245_220823GEN/220823_152058/0000/JPsiToEE_13p6_500_GEN_$fNum.root" outputFileName="TestJPsiToEE_$1_$2_$fNum.root"
	cp TestJPsiToEE_$1_$2_$fNum.root /user/asahasra/CMSSW_12_4_9/src/EDMGenAnalyzer/EDMGenAnalyzerJPsiToEE/test/
    fi
done
