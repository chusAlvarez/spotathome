FROM chusalvarez/spotathome
USER dev
WORKDIR /home/dev
COPY . spotathome
USER root
RUN chown -R dev:dev spotathome
USER dev
WORKDIR /home/dev/spotathome
RUN chmod +x launcher.sh
RUN make clean
RUN export LD_LIBRARY_PATH=/usr/local/lib && make buildmain
ENV LD_LIBRARY_PATH /usr/local/lib
EXPOSE 80
CMD ["/home/dev/spotathome/launcher.sh"]
