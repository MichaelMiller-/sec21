import { Entity, PrimaryGeneratedColumn, Column, ManyToOne, Generated, OneToOne, JoinColumn } from "typeorm";
import { Material } from "./Material";
import { Project } from "./Project";

@Entity()
export class CrossSection {

   @PrimaryGeneratedColumn()
   id: number;

   // @PrimaryGeneratedColumn("uuid")
   // id: string;

   @Column()
   name: string;

   @Column()
   @Generated("uuid")
   uuid: string;

   @OneToOne(type => Material)
   @JoinColumn()
   material: Material;

   //! \todo: real
   @Column()
   area: number;

   @ManyToOne(type => Project, project => project.crossSections)
   project: Project;
}