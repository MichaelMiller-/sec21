import { Entity, PrimaryGeneratedColumn, Column, ManyToOne, Generated } from "typeorm";
import { Material } from "./Material";
import { Project } from "./Project";

@Entity()
export class CrossSection {

   @PrimaryGeneratedColumn()
   id: number;

   @Column()
   name: string;

   @Column()
   @Generated("uuid")
   uuid: string;
   // @PrimaryGeneratedColumn("uuid")
   // id: string;

   @ManyToOne(type => Material, material => material.crossSections)
   material: Material;

   @Column("real")
   area: number;

   @ManyToOne(type => Project, project => project.crossSections)
   project: Project;
}