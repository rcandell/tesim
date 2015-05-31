
plant<-read.table("mc_20150529_plant.dat",header=TRUE,sep="\t",fill=TRUE)
summary(plant)
str(plant)

# select unique pq combinations
pq<-cbind(plant$xmeas_p,plant$xmeas_q,plant$xmv_p,plant$xmv_q)
colnames(pq)<-c("xmeas_p","xmeas_q","xmv_p","xmv_q")
pq_u<-unique(pq)
str(pq)
str(pq_u)
summary(pq_u)

require(lattice)

histogram(plant[plant$xmeas_q<0.3|plant$xmeas_p>0.6,]$Reactor.Pressure..kPa.)

plot(plant[plant$xmeas_q<0.3|plant$xmeas_p>0.6,]$Reactor.Pressure..kPa.)

# find unique pq tuples, produce KPI's for each column after pq's
# could use apply, lapply, etc
# KPI's will include: mean, std, histogram, ITAE, IAE, Shutdowns, when, why
# produce a table of the results

# repeat the above for selected IDV's
# IDV's will include the following:


