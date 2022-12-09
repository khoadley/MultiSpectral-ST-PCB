library(ggplot2)
library(gplots)
library(car)
library(MVN)
library(MASS)
library(data.table)
library(tidyverse)
library(nls2)
library(drc)
library(nlme)

rm(list=ls())

photo<-function(all, name)
{	
	part<-all
	part1add<-list()	
	lev<-levels(factor(part$col))
	for(Y in 1:5)
	{
		part1<-subset(part, col==lev[Y])
		minF<-range(part1$Fo, na.rm=TRUE)[1]
		maxF<-range(part1$Fm, na.rm=TRUE)[2]
		maxSig<-range(part1$Sig, na.rm=TRUE)[2]
		NPQ<-(maxF-part1$Fm)/part1$Fm
	    qP<-((maxF-part1$Fo)/(maxF-minF))
	    ABQ<-(1-(part1$Sig/maxSig))
	    part1add[[Y]]<-data.frame(part1, "NPQ"=NPQ, "qP"=qP, "ABQ"=ABQ)
	}
	all<-data.frame(do.call(rbind, part1add))
	part<-all
	summary(part)
	
	names<-paste("images/", name, ".pdf", sep="")
	levels(as.factor(part$sample))
	pdf(file = names, width = 5, height = 7, bg="transparent")
	par(mfrow=c(9,4), oma = c(2, 3.5, 0.1, 2.5), mar = c(0.1, 0.1, 0.1, 0.1))
	col<-subset(part, col=="Col0norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$light, type="l", title="415nm", ylab="Light Protocol", axes=FALSE)
	col<-subset(part, col=="Col1norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$light, type="l", title="447nm", axes=FALSE)
	col<-subset(part, col=="Col2norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$light, type="l", title="470nm", axes=FALSE)
	col<-subset(part, col=="Col3norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$light, type="l", title="505nm", axes=FALSE)
	col<-subset(part, col=="Col4norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$light, type="l", title="525nm", axes=FALSE)
	
	col<-subset(part, col=="Col0norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$qP, type="l", ylim=c(0,1.0), ylab="qP", axes=FALSE)
	col<-subset(part, col=="Col1norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$qP, type="l", ylim=c(0,1.0), axes=FALSE)
	col<-subset(part, col=="Col2norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$qP, type="l", ylim=c(0,1.0), axes=FALSE)
	col<-subset(part, col=="Col3norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$qP, type="l", ylim=c(0,1.0), axes=FALSE)
	col<-subset(part, col=="Col4norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$qP, type="l", ylim=c(0,1.0), axes=FALSE)
	
	col<-subset(part, col=="Col0norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$NPQ, type="l", ylim=c(0,1.0), ylab="NPQ", axes=FALSE)
	col<-subset(part, col=="Col1norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$NPQ, type="l", ylim=c(0,1.0), axes=FALSE)
	col<-subset(part, col=="Col2norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$NPQ, type="l", ylim=c(0,1.0), axes=FALSE)
	col<-subset(part, col=="Col3norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$NPQ, type="l", ylim=c(0,1.0), axes=FALSE)
	col<-subset(part, col=="Col4norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$NPQ, type="l", ylim=c(0,1.0), axes=FALSE)
	
	col<-subset(part, col=="Col0norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$FvFm, type="l", ylim=c(0,0.5), ylab="Quantum yield PSII", axes=FALSE)
	col<-subset(part, col=="Col1norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$FvFm, type="l", ylim=c(0,0.5), axes=FALSE)
	col<-subset(part, col=="Col2norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$FvFm, type="l", ylim=c(0,0.5), axes=FALSE)
	col<-subset(part, col=="Col3norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$FvFm, type="l", ylim=c(0,0.5), axes=FALSE)
	col<-subset(part, col=="Col4norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$FvFm, type="l", ylim=c(0,0.5), axes=FALSE)
	
	col<-subset(part, col=="Col0norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$Sig, type="l", ylim=c(0,10.0), ylab="Sigma", axes=FALSE)
	col<-subset(part, col=="Col1norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$Sig, type="l", ylim=c(0,10.0), axes=FALSE)
	col<-subset(part, col=="Col2norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$Sig, type="l", ylim=c(0,10.0), axes=FALSE)
	col<-subset(part, col=="Col3norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$Sig, type="l", ylim=c(0,10.0), axes=FALSE)
	col<-subset(part, col=="Col4norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$Sig, type="l", ylim=c(0,10.0), axes=FALSE)
	
	col<-subset(part, col=="Col0norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$ABQ, type="l", ylim=c(0,1.25), ylab="ABQ", axes=FALSE)
	col<-subset(part, col=="Col1norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$ABQ, type="l", ylim=c(0,1.25), axes=FALSE)
	col<-subset(part, col=="Col2norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$ABQ, type="l", ylim=c(0,1.25), axes=FALSE)
	col<-subset(part, col=="Col3norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$ABQ, type="l", ylim=c(0,1.25), axes=FALSE)
	col<-subset(part, col=="Col4norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$ABQ, type="l", ylim=c(0,1.25), axes=FALSE)
	
	col<-subset(part, col=="Col0norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$P, type="l", ylim=c(0,0.95), ylab="connectivity", axes=FALSE)
	col<-subset(part, col=="Col1norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$P, type="l", ylim=c(0,0.95), axes=FALSE)
	col<-subset(part, col=="Col2norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$P, type="l", ylim=c(0,0.95), axes=FALSE)
	col<-subset(part, col=="Col3norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$P, type="l", ylim=c(0,0.95), axes=FALSE)
	col<-subset(part, col=="Col4norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$P, type="l", ylim=c(0,0.95), axes=FALSE)
	
	col<-subset(part, col=="Col0norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$Tau1, type="l", ylim=c(0,7450), ylab="Tau1", axes=FALSE)
	col<-subset(part, col=="Col1norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$Tau1, type="l", ylim=c(0,7450), axes=FALSE)
	col<-subset(part, col=="Col2norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$Tau1, type="l", ylim=c(0,7450), axes=FALSE)
	col<-subset(part, col=="Col3norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$Tau1, type="l", ylim=c(0,7450), axes=FALSE)
	col<-subset(part, col=="Col4norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$Tau1, type="l", ylim=c(0,7450), axes=FALSE)
	
	col<-subset(part, col=="Col0norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$Tau2, type="l", ylim=c(0,6595000), ylab="Tau2", axes=FALSE)
	col<-subset(part, col=="Col1norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$Tau2, type="l", ylim=c(0,6595000), axes=FALSE)
	col<-subset(part, col=="Col2norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$Tau2, type="l", ylim=c(0,6595000), axes=FALSE)
	col<-subset(part, col=="Col3norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$Tau2, type="l", ylim=c(0,6595000), axes=FALSE)
	col<-subset(part, col=="Col4norm" & sample==levels(as.factor(part$sample))[1])
	plot(col$time, col$Tau2, type="l", ylim=c(0,6595000), axes=FALSE)
	
	dev.off()
}


movingAverage <- function(x, n=1, centered=FALSE) 
{  
    if (centered) {
        before <- floor  ((n-1)/2)
        after  <- ceiling((n-1)/2)
    } else {
        before <- n-1
        after  <- 0
    }
    s     <- rep(0, length(x))
    count <- rep(0, length(x))
    new <- x 
    count <- count + !is.na(new)
    new[is.na(new)] <- 0
    s <- s + new
    i <- 1
    while (i <= before) {
        new   <- c(rep(NA, i), x[1:(length(x)-i)])
        count <- count + !is.na(new)
        new[is.na(new)] <- 0
        s <- s + new
        i <- i+1
    }
    i <- 1
    while (i <= after) {
        # This is the vector with offset values to add
        new   <- c(x[(i+1):length(x)], rep(NA, i))
        count <- count + !is.na(new)
        new[is.na(new)] <- 0
        s <- s + new
        i <- i+1
    }
    s/count
}



curver<-function(yvalue, Fo, Fm, C, sizer)
{		len<-round(length(yvalue)/sizer,0)
		if (len<3)
		{	len <-5
		}
		satcurve1 <- function(P, Ces, Foo) 
		{   Cd<-C
      		Ye<-Foo
      		for (O in 2:len) 
      		{   Ct<-Cd+Ces*((1-Cd)/(1-Cd*P))
      			Yv<-Foo + (Fm - Foo)*Ct*((1-P)/(1-(Ct*P)))
          		Ye<-append(Ye, Yv)
          		Cd<-Ct
      		}
      		return(Ye)
   		}
   		STR<-yvalue[1:len]
		fit2 <- try(nls2(STR~satcurve1(P, Ces, Foo), start=list(P=0.01, Ces=Cest, Foo=Fo), algorithm="port", control = list(maxiter = 2000), lower=c(0,0,Fo*0.995), upper=c(1,1,Fo*1.005)), silent=TRUE)	#find a better P estimate?
		if(mode(fit2)=="character")
		{	fit2 <- try(nls2(STR~satcurve1(P, Ces, Foo), start=list(P=0.35, Ces=Cest, Foo=Fo), algorithm="port", control = list(maxiter = 2000), lower=c(0,0,Fo*0.995), upper=c(1,1,Fo*1.005)), silent=TRUE)	
		}
		if(mode(fit2)=="character")
		{	fit2 <- try(nls2(STR~satcurve1(P, Ces, Foo), start=list(P=0.7, Ces=Cest, Foo=Fo), algorithm="port", control = list(maxiter = 2000), lower=c(0,0,Fo*0.995), upper=c(1,1,Fo*1.005)), silent=TRUE)	
		}
		summary(fit2)
		len<-length(yvalue)
		satcurve2 <- function(P, Fm) 
		{   Fo<-coef(fit2)[3]
			Ces<-coef(fit2)[2]
			Cd<-C
      		Ye<-Fo
      		for (O in 2:len) 
      		{   Ct<-Cd+Ces*((1-Cd)/(1-Cd*P))
      			Yv<-Fo + (Fm - Fo)*Ct*((1-P)/(1-(Ct*P)))
          		Ye<-append(Ye, Yv)
          		Cd<-Ct
      		}
      		return(Ye)
   		}
		fit3 <- nls2(yvalue~satcurve2(P, Fm), start=list(P=coef(fit2)[1], Fm=Fm), algorithm="port", control = list(maxiter = 2000), lower=c(0,Fm*0.995), upper=c(1,Fm*1.005))	
		summary(fit3)
		
		len<-length(yvalue)
		satcurve3 <- function(P, Ces, Fo) 
		{   Fm<-coef(fit3)[2]
			Cd<-C
      		Ye<-Fo
      		for (O in 2:len) 
      		{   Ct<-Cd+Ces*((1-Cd)/(1-Cd*P))
      			Yv<-Fo + (Fm - Fo)*Ct*((1-P)/(1-(Ct*P)))
          		Ye<-append(Ye, Yv)
          		Cd<-Ct
      		}
      		return(Ye)
   		}
		fit4 <- nls2(yvalue~satcurve3(P, Ces, Fo), start=list(P=coef(fit3)[1], Ces=coef(fit2)[2], Fo=coef(fit2)[3]), algorithm="port", control = list(maxiter = 2000), lower=c(0,0,(0.995*coef(fit2)[3])), upper=c(1,1,(1.005*coef(fit2)[3])))
		fit5 <- try(nls2(yvalue~satcurve3(P, Ces, Fo), start=coef(fit4), algorithm="port", control = list(maxiter = 2000), lower=c(0,0,(0.995*coef(fit2)[3])), upper=c(1,1,(1.005*coef(fit2)[3]))), silent=TRUE)
		summary(fit5)
		finalDat<-rbind(summary(fit2)$coefficients, summary(fit3)$coefficients, summary(fit5)$coefficients) 
		return(finalDat)
}
							
							
###########			
###########
###REF#####
###########
###########

RefFile<-readLines("data/Jun9-Palau22/Ref1.CSV")
Refcapture<-list()
V<-1 
for (G in 1:(length(RefFile)-2))
{
	chart<-unlist(strsplit(RefFile[G], ","))
	if (chart[1]=="Time")
	{	Refcapture[[V]]<-RefFile[G:(G+4)]
		V<-V+1
	}
}

###########
###########
###########										
							
datas<-list()
foldernames <- list.files("data", full.names=TRUE)   ######Folder name
for (V in 1:length(foldernames))
{	
	if(grepl("Nikko-Palau22", foldernames[V], perl=TRUE)=="TRUE")
	{	BB<-0
		numb = 0
		all<-list()
		Woo<-length(list.files(foldernames[V], full.names=TRUE))
		filenames<-list.files(foldernames[V], full.names=TRUE)
		for(F in 1:Woo)
		{	if(grepl("R.CSV", filenames[F], perl=TRUE)=="TRUE")
			{	BB<-BB+1
				Namer<-unlist(strsplit(unlist(strsplit(filenames[F], "/"))[3], "R.CS"))[1]
				Dater<-unlist(strsplit(filenames[F], "/"))[2]				
				#ICalls<-read.csv(filenames[F], header=FALSE, skip=2)
				ICalls<-readLines(filenames[F])
				capture<-list()
				V<-1 
				for (G in 1:(length(ICalls)-2))
				{
					chart<-unlist(strsplit(ICalls[G], ","))
					if (chart[1]=="Time")
					{	capture[[V]]<-ICalls[G:(G+4)]
						V<-V+1
					}
				}
				par(mfrow=c(4,2), oma = c(2, 3.5, 2.1, 2.5), mar = c(0.1, 0.1, 1.1, 0.1))
				############
				PAR<-list()
				for (I in 1:length(capture))
				{	
					print(paste(I, "of", length(capture), "sample", Namer, BB, "of", Woo/2, sep=" "))
					MULTIS<-list()
					meta<-capture[[I]][1]
					dat<-unlist(strsplit(meta, ","))
					time<-as.numeric(dat[8])
					light<-as.numeric(dat[9])
					year<-as.numeric(dat[2])
					month<-as.numeric(dat[3])
					day<-as.numeric(dat[4])
					hour<-as.numeric(dat[5])
					minute<-as.numeric(dat[6])
					second<-as.numeric(dat[7])
					stepper<-I
					for (G in 2:length(capture[[I]]))
					{	
						simple<-na.omit(as.matrix(as.numeric(unlist(strsplit(capture[[I]][G], "\\,"))[-1])))
						leny<-length(simple)-60
						if (G == "2")
						{	excit<-20200}
						if (G == "3")
						{   excit<-30620}
						if (G == "4")
						{   excit<-17050}
						if (G == "5")
						{   	excit<-19500}
						if(I<24)
						{   Refsimple<-na.omit(as.matrix(as.numeric(unlist(strsplit(Refcapture[[I]][G], "\\,"))[-1])))[1:leny]
						}
						else
						{	Refsimple<-na.omit(as.matrix(as.numeric(unlist(strsplit(Refcapture[[1]][G], "\\,"))[-1])))[1:leny]
						}

						simplex<-1+(1-(Refsimple/Refsimple[1]))
						col<-unlist(strsplit(capture[[I]][G], "\\,"))[1]
						simpery<-simple[1:leny]
						simper<-simpery*simplex
						samp<-length(simper)
						x<-c(1.5)
						n <-1.5
						for (i in 1:(samp[1]-1))
						{	n <- (n + 4.01)
							x <- append(x, n)
						}
						xx <- as.matrix(x)
						x1<- xx
						yvalue<-simper
						endo<-samp[1]
						scores1<-list()
						for(U in 10:(endo))
						{	srt<-endo-U
							end<-srt+10
							x1<- xx[srt:end]
							yvalue<- simper[srt:end]
							scores1[[U]]<-data.frame("distt"=srt,"squarer"=abs(lm(yvalue~x1)[[1]][2]))
						}
						res1 <- data.frame(do.call(rbind, scores1))
						finalist1<-res1[order(res1[,2]),][1,1]
						if(is.numeric(finalist1)==TRUE)
						{	end0<-finalist1+10
							x1<- xx[1:end0]
							yvalue<- simper[1:end0]
						}
						if(is.numeric(finalist1)==FALSE)
						{	x1<- xx[1:endo]
							yvalue<- simper[1:endo]
						}
						low<-yvalue[1]
						high<-yvalue[length(yvalue)]
						
						v <- -999
						cur<-yvalue ~ A + (B-A)*(1-exp(-(C*x1)))
						st2 <- data.frame(A = c(low*0.85, low*1.15), B = c(high*0.85, high*1.15), C = c(-0.1, 0.1)) 
						mod2 <- try(nls2(cur, start = st2, algorithm = "brute-force", control=list(maxiter=300)), silent=TRUE)
						v<-try(nls2(cur, start = coef(mod2), control = list(maxiter = 4000), algorithm = "default"), silent=TRUE)		

						if(mode(v)=="character")
						{	coef <- NA
							Aest <- NA
							Best <- NA
							Cest <- NA
							Aerr <- NA
							Berr <- NA
							Cerr <- NA
							Fv <- NA
							Fm <- NA
							Fo <- NA
							sigma <- NA
							Fv_Fm <- NA	
							Spval <- NA
							Fopval <- NA
							Fmpval <- NA
							conpval <- NA
							P<-NA
							Pval<-NA		
							
							plot(x1, yvalue, pch=19, cex=0.5, col="black", main=col)
						}
						if (mode(v)=="list")
						{	coef <- summary(v)$coefficients
							Aest <- coef[1,1]
							Best <- coef[2,1]
							Cest <- coef[3,1]
								
							Fo <- Aest + (Best-Aest)*(1-exp(-(Cest*1)))
							Fm <- Aest + (Best-Aest)*(1-exp(-(Cest*x1[length(x1)])))
							qP<- (Fm-yvalue)/(Fm-Fo)
							CC<-1-qP	
							
							sigma2<-Cest/excit*1000000
							sigmaPval<-coef[3,4]

							X<-1.0
							ResSat<-list()
							for(Y in 1:12)
							{   TE<-try(curver(yvalue, yvalue[1], Fm, CC[1], X), silent=TRUE)
								if(mode(TE)=="numeric")
								{
									RES<-data.frame("x"=X, TE)
									ResSat[[Y]]<-data.frame("values"=rownames(RES)[5:8], RES[5:8,], "sterr"=sum(RES[5:8,3]), "pval"=sum(RES[5:8,5]))
								}
								if(mode(TE)=="character")
								{	st<-rbind(c(NA,NA,NA,NA,NA),c(NA,NA,NA,NA,NA),c(NA,NA,NA,NA,NA),c(NA,NA,NA,NA,NA))
									colnames(st)<-c("x", "Estimate", "Std..Error", "t.value", "Pr...t..")
									ResSat[[Y]]<-data.frame("values"=c("Fm", "P.2", "Ces.1", "Fo"), st, "sterr"=NA, "pval"=NA)
								}
								
								
								X<-X+0.5
							}
							allerie2<-data.frame(do.call(rbind, ResSat))
							all2<-subset(allerie2, values=="P.2")
							all2[order(all2[,6]),][1,2]
							XX<-all2[order(all2[,6]),][1,2]
							if(!is.numeric(XX))
							{	Fm<-NA
								P<-NA
								Fo<-NA
								Fv_Fm<-NA
								sigma<-NA
								Fopval<-NA
								Fmpval<-NA
								Spval<-NA
								Pval<-NA
								plot(x1, yvalue, pch=19, cex=0.5, col="black", main=col)
							}
							if(is.numeric(XX))
							{	finalVal<-subset(allerie2, x==XX)	
								len<-length(yvalue)
								Fm<-finalVal[1,3]
								P<-finalVal[2,3]
								Ces<-finalVal[3,3]
								Fo<-finalVal[4,3]
								Cd<-CC[1]
					      		Ye<-Fo
					      		for (O in 2:len) 
					      		{   Ct<-Cd+Ces*((1-Cd)/(1-Cd*P))
					      			Yv<-Fo + (Fm - Fo)*Ct*((1-P)/(1-(Ct*P)))
					          		Ye<-append(Ye, Yv)
					          		Cd<-Ct
					      		}
								Fopval<-finalVal[4,6]
								Fmpval<-finalVal[1,6]
								Spval<-finalVal[3,6]
								Pval<-finalVal[2,6]
									
								Fv <- Fm-Fo
								sigma <- Ces/excit*1000000
								Fv_Fm <- Fv/Fm
								Ft<-Fo*0.95
								max<-Fm*1.05
									
								plot(x1, yvalue, pch=19, cex=0.5, col="black", ylim=c(Ft,max), main=col)
								lines(x1,Ye, col ="red", lwd=1.75)
							}
							
						}
						strt<-leny+1
						tau<-simple[strt:length(simple)]
						dimper<-as.matrix(as.numeric(tau))
						damp<-dim(dimper)
						
						x<-list()
						xx<-list()
						x[[1]] <-0
						xx[[1]] <-0
						x[[2]] <-50
						xx[[2]] <-50
						for (i in 3:(damp[1]))
						{	xx[[i]] <- (xx[[i-1]]*1.1)
							x[[i]] <- xx[[i]]+x[[i-1]]
						}
						xx <- as.matrix(do.call(rbind, x))
						means<-median(dimper, na.rm=TRUE)
						sds<-sd(dimper, na.rm=TRUE)
						yval<-list()
						for(R in 1:length(dimper))
						{	if(is.na(dimper[R])==TRUE)
							{
								yval[[R]]<-NA
							}
							else if(dimper[R] < (simper[1]*0.3))
							{
								yval[[R]]<-NA
							}
							else if(dimper[R] > (simper[strt-2]*1.5))
							{
								yval[[R]]<-NA
							}
							else if(dimper[R] < (means+sds*5))
							{
								yval[[R]]<-dimper[R]
							}	
							else if(dimper[R] > (means-sds*5))
							{
								yval[[R]]<-dimper[R]
							}	
						}
						yx <- as.matrix(do.call(rbind, yval))
						frame<-na.omit(data.frame(xx,"yx"=yx))
						summary(frame)
						fin<-dim(frame)[1]
						x1<-frame$xx
						yvalue<-frame$yx
						if(is.numeric(Fm)==TRUE)
						{	Aest<-Fm
							Cest<-Fm-Fv*0.5
							Eest<-Fo+Fv*0.15
						}
						if(is.numeric(Fm)==FALSE)
						{	Fm<-simper[strt-1]
							Fo<-simper[2]
							Fv<-Fm-Fo
							Aest<-Fm
							Cest<-Fm-Fv*0.5
							Eest<-Fo+Fv*0.15
						}					
						lowE<-0.75
						hiE<-1.25
						finn<-fin+1
						scores1<-list()
						for(U in seq(1,15,1))
						{	end0<-finn-U
							x1<- frame$xx[1:end0]
							yvalue<- frame$yx[1:end0]
							cur<-yvalue~(A*(exp(-x1/B)))+(C*(exp(-x1/D)))
							st2 <- list(A=c(Aest*lowE,Aest*hiE), B=c(100,1000), C=c(Cest*lowE,Cest*hiE), D=c(1000,30000))
							model1<-try(nls2(cur, start = st2, algorithm = "brute-force", control=list(maxiter=50)), silent=TRUE)
							model<--999
							model<-try(nls2(cur, start = coef(model1), control = list(maxiter = 10000), algorithm = "default"), silent=TRUE)
							if(mode(summary(model))=="list")
							{	#plot(model, log="")
								estT1<-summary(model)$coefficients
								scores1[[U]]<-data.frame("distt"=U,"squarer"=sum(estT1[,2]))
							}
							else if(mode(summary(model))=="character")
							{	scores1[[U]]<-data.frame("distt"=U, "squarer"=NA)
							}
						}
						res1 <- data.frame(do.call(rbind, scores1))
						finalist1<-res1[order(res1[,2]),][1,1]
						
						if(is.numeric(finalist1)==TRUE)
						{	end0<-finn-finalist1
							x1<- frame$xx[1:end0]
							yvalue<- frame$yx[1:end0]
						}
						if(is.numeric(finalist1)==FALSE)
						{	end0<-fin
							x1<- frame$xx[1:end0]
							yvalue<- frame$yx[1:end0]
						}
						cur<-yvalue~(A*(exp(-x1/B)))+(C*(exp(-x1/D)))	
						st2 <- list(A=c(Aest*lowE,Aest*hiE), B=c(100,1000), C=c(Cest*lowE,Cest*hiE), D=c(1000,30000))					
						fited1<-try(nls2(cur, start = st2, algorithm = "brute-force", control=list(maxiter=400)), silent=TRUE)
						v<--999
						v<-try(nls2(cur, start = coef(fited1), control = list(maxiter = 10000), algorithm = "default"), silent=TRUE)	
						if(mode(v)=="character")
						{	Tau1<-NA
							Tau2<-NA
							TauAve<-NA
							TauFo<-NA
							T1pval<-NA
							T2pval<-NA
							plot(x1, yvalue, pch=19, cex=0.5, col="black", main="no-fit")
						}
						if(mode(v)=="list")
						{	COEF<-summary(v)$coefficients
							Acoe<-COEF[1,1]
							Bcoe<-COEF[2,1]
							Ccoe<-COEF[3,1]
							Dcoe<-COEF[4,1]
							T1pval<-COEF[2,4]
							T2pval<-COEF[4,4]
							rest<-(Acoe*(exp(-x1/Bcoe)))+(Ccoe*(exp(-x1/Dcoe)))
							Tau1<-Bcoe
							Tau2<-Dcoe
							TauAve<-(Bcoe+Dcoe)/2
							TauFo<-(Acoe*(exp(-x1[end0]/Bcoe)))+(Ccoe*(exp(-x1[end0]/Dcoe)))							
							hh<-rest[1]*0.9
							ll<-rest[length(rest)]*1.1
							plot(x1, yvalue, pch=19, cex=0.5, col="black", log="x")
							lines(x1,rest, col ="red", lwd=1.75)	
						}						
						MULTIS[[G]]<-data.frame("dater"=Dater, "sample"=Namer, "col"=col, "year"=year, "month"=month, "day"=day, "hour"=hour, "minute"=minute, "second"=second, "light"=light, "time"=time, "Lstep"=stepper, "Fo"=Fo, "Fm"=Fm, "FvFm"=Fv_Fm, "Sig"=sigma, "P"=P, "TauFo"=TauFo,"Tau1"= Tau1, "Tau2"= Tau2, "TauAve"= TauAve, "Spval"=Spval, "Fopval"=Fopval, "Fmpval"=Fmpval, "Pval"=Pval, "T1pval"=T1pval, "T2pval"=T2pval, "sigma2"=sigma2, "sigmaPval"=sigmaPval)
					}	
				PAR[[I]]<-data.frame(do.call(rbind, MULTIS))	
				}
			all[[BB]]<-data.frame(do.call(rbind, PAR))
			NN<-MULTIS[[G]][2]
			photo(all[[BB]], NN)
			}
		}
	}
}
datas<-data.frame(do.call(rbind, all))
summary(datas)
dim(datas)
							
write.csv(datas, "Palau22-Nikko.csv")





