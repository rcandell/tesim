
calc_vitae<-function(v, sp, tm )
{
  deltaT<-diff(tm,1)
  deltaT[[length(deltaT)]]<-deltaT[[length(deltaT)-1]]
  errs<-sp-v
  itae<-cumsum(tm*abs(errs)*deltaT)
  itae
}

calc_itae<-function(v, sp, tm )
{
  deltaT<-diff(tm,1)
  deltaT[[length(deltaT)]]<-deltaT[[length(deltaT)-1]]
  errs<-sp-v
  itae<-sum(tm*abs(errs)*deltaT)
  itae
}

calc_iae<-function(v, tm, sp )
{
  deltaT<-diff(tm,1)
  deltaT[[length(deltaT)]]<-deltaT[[length(deltaT)-1]]
  errs<-sp-v
  iae<-sum(abs(errs)*deltaT)
  iae
}

calc_msae<-function(v, tm, sp )
{
  deltaT<-diff(tm,1)
  deltaT[[length(deltaT)]]<-deltaT[[length(deltaT)-1]]
  errs<-sp-v
  errssq<-errs^2
  sum(errssq[1:length(deltaT)]*deltaT[1:length(deltaT)])
}

kpidf<-function(df, tm, sp)
{
  kpis<-sp

  for(nn in sp$sp_name)
  {
    v<-df[nn]
    kpis$msae[sp_name==nn]   <-  calc_msae(v=v, tm = tm, sp = sp[sp_name==nn,]$sp_value)
    kpis$iae[sp_name==nn]    <-  calc_iae(v=v, tm = tm, sp = sp[sp_name==nn,]$sp_value)
    kpis$itae[sp_name==nn]   <-  calc_itae(v=v, tm = tm, sp = sp[sp_name==nn,]$sp_value)
    kpis$uae[sp_name==nn]    <-  sapply(v-sp[sp_name==nn,]$sp_value, mean)
    kpis$varae[sp_name==nn]  <-  sapply(v-sp[sp_name==nn,]$sp_value, var)
    kpis$sdae[sp_name==nn]   <-  sapply(v-sp[sp_name==nn,]$sp_value, sd)
    kpis$maxae[sp_name==nn]  <-  max   (v-sp[sp_name==nn,]$sp_value)
    kpis$minae[sp_name==nn]  <-  min   (v-sp[sp_name==nn,]$sp_value)
    kpis$medae[sp_name==nn]  <-  sapply(v-sp[sp_name==nn,]$sp_value, median)
  }
  kpis
}


