//****************************************Copyright (c)***********************************//
//原子哥在线教学平台：www.yuanzige.com
//技术支持：www.openedv.com
//淘宝店铺：http://openedv.taobao.com
//关注微信公众平台微信号："正点原子"，免费获取ZYNQ & FPGA & STM32 & LINUX资料。
//版权所有，盗版必究。
//Copyright(C) 正点原子 2018-2028
//All rights reserved
//----------------------------------------------------------------------------------------
// File name:           bram_rd
// Last modified Date:  2019/10/8 17:25:36
// Last Version:        V1.0
// Descriptions:        读BRAM模块
//----------------------------------------------------------------------------------------
// Created by:          正点原子
// Created date:        2019/10/8 17:25:36
// Version:             V1.0
// Descriptions:        The original version
//
//----------------------------------------------------------------------------------------
//****************************************************************************************//
 
module bram_rd(
    input                clk        , //时钟信号
    input                rst_n      , //复位信号
    input                start_rd   , //读开始信号
    input        [31:0]  start_addr , //读开始地址  
    input        [31:0]  rd_len     , //读数据的长度
    //RAM端口
    output               ram_clk    , //RAM时钟
    input        [31:0]  ram_rd_data, //RAM中读出的数据
    output  reg          ram_en     , //RAM使能信号
    output  reg  [31:0]  ram_addr   , //RAM地址
    output  reg  [3:0]   ram_we     , //RAM读写控制信号
    output  reg  [31:0]  ram_wr_data, //RAM写数据
    output               ram_rst      //RAM复位信号,高电平有效
);

//reg define
reg  [1:0]   flow_cnt;
reg          start_rd_d0;
reg          start_rd_d1;

//wire define
wire         pos_start_rd;

//*****************************************************
//**                  main code
//*****************************************************

assign  ram_rst = 1'b0;
assign  ram_clk = clk ;
assign pos_start_rd = ~start_rd_d1 & start_rd_d0;

//延时两拍，采start_rd信号的上升沿
always @(posedge clk or negedge rst_n) begin
    if(!rst_n) begin
        start_rd_d0 <= 1'b0;   
        start_rd_d1 <= 1'b0; 
    end
    else begin
        start_rd_d0 <= start_rd;   
        start_rd_d1 <= start_rd_d0;     
    end
end

//根据读开始信号,从RAM中读出数据
always @(posedge clk or negedge rst_n) begin
    if(!rst_n) begin
        flow_cnt <= 2'd0;
        ram_en <= 1'b0;
        ram_addr <= 32'd0;
        ram_we <= 4'd0;
    end
    else begin
        case(flow_cnt)
            2'd0 : begin
                if(pos_start_rd) begin
                    ram_en <= 1'b1;
                    ram_addr <= start_addr;
                    flow_cnt <= flow_cnt + 2'd1;
                end
            end
            2'd1 : begin
                if(ram_addr - start_addr == rd_len - 4) begin  //数据读完
                    ram_en <= 1'b0;
                    flow_cnt <= flow_cnt + 2'd1;
                end
                else
                    ram_addr <= ram_addr + 32'd4;              //地址累加4
            end
            2'd2 : begin
                ram_addr <= 32'd0; 
                flow_cnt <= 2'd0;
            end
        endcase    
    end
end

endmodule
