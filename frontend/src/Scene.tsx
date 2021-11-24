import React, {useCallback, useMemo, useState} from 'react';
import {DbCurveMember, DbStructuralPoint} from "./DatabaseInterface";
import './App.css'
import {Canvas, useFrame, useThree} from '@react-three/fiber';
import {Vector2, Vector3} from 'three';

type DotProps = {
    position: Vector3
    name: string
}

const Dot = (props: DotProps) => {
    const [hovered, setHover] = useState(false);
    const [clicked, setClick] = useState(false)

    return (
        <mesh position={props.position}>
            onClick={() => setClick(!clicked)}
            onPointerOver={() => setHover(true)}
            onPointerOut={() => setHover(false)}
            <circleGeometry args={[0.09, 16]}/>
            <meshBasicMaterial color={hovered ? 'hotpink' : 'orange'}/>
        </mesh>
    )
}

type LineProps = {
    begin: Vector3
    end: Vector3
}
const Line2 = (props: LineProps) => {
    const points = useMemo(() => [props.begin, props.end], [props.begin, props.end])
    const onUpdate = useCallback(self => self.setFromPoints(points), [points])

    return (
        <line>
            <bufferGeometry onUpdate={onUpdate}/>
            <lineBasicMaterial color={'green'} linewidth={30}/>
        </line>
    )
}

type CameraControlProps = {
    zoom: number
    deltaDrag: Vector2
}
const CameraControls = (props: CameraControlProps) => {
    // const {camera, size: {width, height}} = useThree();
    const {camera } = useThree();

    const dragVelocity = 100000

    return useFrame(() => {
        const X = camera.position.x + (props.deltaDrag.x / dragVelocity)
        const Y = camera.position.y + (props.deltaDrag.y / dragVelocity)

        const pos = new Vector3(X, Y, props.zoom)
        camera.position.lerp(pos, 0.5)
        camera.updateProjectionMatrix()
    })
}



type SceneProps = {
    spts: DbStructuralPoint[]
    members: DbCurveMember[]
}

const Scene = (props: SceneProps) => {

    const zoomVelocity = 20

    const [zoom, setZoom] = useState(3)
    const [beginDragPosition, setBeginDragPosition] = useState(new Vector2())
    const [dragDistance, setDragDistance] = useState(new Vector2())

    const onMouseDownEvent = (evt: React.MouseEvent<HTMLDivElement>) => {
        setBeginDragPosition(new Vector2(evt.clientX, evt.clientY))
    }
    const onMouseUpEvent = (evt: React.MouseEvent<HTMLDivElement>) => {
        setDragDistance(beginDragPosition.sub(new Vector2(evt.clientX, evt.clientY)))
    }

    return (
        <div className="canvasFrame"
             onWheel={(e) => setZoom(zoom + (e.deltaY / zoomVelocity))}
             onMouseDown={onMouseDownEvent}
             onMouseUp={onMouseUpEvent} >
            <Canvas>
                {props.spts.map(e => (
                    <Dot position={new Vector3(e.coordinate_x, e.coordinate_y, e.coordinate_z)} name={e.name}/>))}

                {props.members.map((e: DbCurveMember) => (
                    <Line2 begin={new Vector3(e.beginNode.coordinate_x,e.beginNode.coordinate_y,e.beginNode.coordinate_z)}
                           end={new Vector3(e.endNode.coordinate_x,e.endNode.coordinate_y,e.endNode.coordinate_z)} />
                ))}
                <CameraControls zoom={zoom} deltaDrag={dragDistance}/>
            </Canvas>
        </div>
    );
}

export default Scene