###########################################################
########   TO RUN THIS: python submit_calo.py      ########
########   DO NOT DO: crab submit submit_calo.py   ########
###########################################################


from CRABClient.UserUtilities import config
from httplib import HTTPException
from multiprocessing import Process
from CRABAPI.RawCommand import crabCommand

def submit(config):
    try:
        crabCommand('submit', config = config)
    except HTTPException, hte:
        print 'Cannot execute command'
        print hte.headers

if __name__ == '__main__':
    Samples = [ 
        '/ScoutingCaloMuon/Run2017C-v1/RAW',
        '/ScoutingCaloMuon/Run2017D-v1/RAW',
        '/ScoutingCaloMuon/Run2017E-v1/RAW',
        '/ScoutingCaloMuon/Run2017F-v1/RAW'
    ]
    
    config = config()

    version = 'v1'

    config.General.requestName = ''
    config.General.workArea = 'jobs_4mu'

    config.JobType.pluginName = 'Analysis'
    config.JobType.psetName = '../python/ScoutingNtuplizer_4mu_cfg.py'
    config.JobType.allowUndistributedCMSSW = True

    config.Data.inputDataset = ''
    config.Data.splitting = 'LumiBased'
    config.Data.unitsPerJob = 500 
    config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions17/13TeV/Final/Cert_294927-306462_13TeV_PromptReco_Collisions17_JSON.txt' 
    config.Data.publication = False
    config.Data.ignoreLocality = False
    
    config.Data.outLFNDirBase = "/store/user/duadams/muon_data/4mu" # if you are not duncan, change this to an area you can write to
    config.Site.storageSite = 'T3_US_Rutgers'


    for dataset in Samples:
        dataset_string = dataset.split('/ScoutingCaloMuon/')[1].split('/RAW')[0]
        dataset_name = dataset_string.split('-')[0]
        version = dataset_string.split('-')[1]
        name = 'ScoutingCaloMuon_Ntuples_{0}_{1}'.format(dataset_name, version)
        config.General.requestName = name
        config.Data.outputDatasetTag = name
        output_name = 'scouting_ntuple.root'
        config.JobType.pyCfgParams = ['outputFile={0}'.format(output_name)]
        config.JobType.outputFiles = [output_name]
        config.Data.inputDataset = dataset
        
        p = Process(target=submit, args=(config,))
        p.start()
        p.join()

